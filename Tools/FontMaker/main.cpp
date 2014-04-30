#include "FontHelper.h"
#include "./Core/include/MathHelpers.h"
#include "./Human/include/Utilities/ImagePNG.h"

#include "./Human/include/Rendering/Font/FontKerning.h"


static OPint OPfontInit(OPfont* font, i8* filename)
{	
	FT_Library library;
	FT_Face face;
	FT_Size_Metrics metrics;

	font->glyphs = OPvectorCreate(sizeof(OPfontGlyph*));
	font->height = 0;
	font->ascender = 0;
	font->descender = 0;
	font->outlineType = 0;
	font->outlineThickness = 0.0;
	font->hinting = 1;
	font->kerning = 1;
	font->filtering = 1;

	// FT_LCD_FILTER_LIGHT   is (0x00, 0x55, 0x56, 0x55, 0x00)
	// FT_LCD_FILTER_DEFAULT is (0x10, 0x40, 0x70, 0x40, 0x10)
	font->lcdWeights[0] = 0x10;
	font->lcdWeights[1] = 0x40;
	font->lcdWeights[2] = 0x70;
	font->lcdWeights[3] = 0x40;
	font->lcdWeights[4] = 0x10;

	/* Get font metrics at high resolution */
	if (!OPfontGetHiResFace(font, filename, &library, &face))
		return -1;

	font->underlinePosition = face->underline_position / (float)(HRESf*HRESf) * font->size;
	font->underlinePosition = OPround(font->underlinePosition);
	if (font->underlinePosition > -2)
	{
		font->underlinePosition = -2.0;
	}

	font->underlineThickness = face->underline_thickness / (float)(HRESf*HRESf) * font->size;
	font->underlineThickness = OPround(font->underlineThickness);
	if (font->underlineThickness < 1)
	{
		font->underlineThickness = 1.0;
	}

	metrics = face->size->metrics;
	font->ascender = (metrics.ascender >> 6) / 100.0f;
	font->descender = (metrics.descender >> 6) / 100.0f;
	font->height = (metrics.height >> 6) / 100.0f;
	font->lineGap = font->height - font->ascender + font->descender;
	FT_Done_Face(face);
	FT_Done_FreeType(library);

	/* -1 is a special glyph */
	OPfontGetGlyph(font, -1);

	return 0;
}


void OPfontKerningGenerate(OPfont* font, i8* filename){
	OPuint i, j;
	FT_Library library;
	FT_Face face;
	FT_UInt glyphIndex, prevIndex;
	OPfontGlyph* glyph, *prevGlyph;
	FT_Vector kerning;

	if (!OPfontGetFace(font, filename, &library, &face)) {
		return;
	}

	for (i = 1; i < font->glyphs->_size; ++i) {
		glyph = *(OPfontGlyph **)OPvectorGet(font->glyphs, i);
		glyphIndex = FT_Get_Char_Index(face, glyph->charcode);
		OPvectorClear(glyph->kerning);
		for (j = 1; j < font->glyphs->_size; ++j) {
			prevGlyph = *(OPfontGlyph **)OPvectorGet(font->glyphs, j);
			prevIndex = FT_Get_Char_Index(face, prevGlyph->charcode);
			FT_Get_Kerning(face, prevIndex, glyphIndex, FT_KERNING_UNFITTED, &kerning);
			if (kerning.x) {
				OPfontKerning k = { prevGlyph->charcode, kerning.x / (OPfloat)(HRESf * HRESf) };
				OPvectorPush(glyph->kerning, (ui8*)&k);
			}
		}
	}

	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

OPint OPfontLoadGlyphs(OPfont* font, i8* filename, i8* charcodes, OPint count)
{
	OPint i, x, y, width, height, depth, w, h;
	FT_Library library;
	FT_Error error;
	FT_Face face;
	FT_Glyph ft_glyph;
	FT_GlyphSlot slot;
	FT_Bitmap ft_bitmap;

	FT_UInt glyph_index;
	OPfontGlyph* glyph;
	OPfontAtlasRegion region;
	size_t missed = 0;

	width = font->atlas->width;
	height = font->atlas->height;
	depth = font->atlas->depth;

	if (!OPfontGetFace(font, filename, &library, &face))
		return count;

	/* Load each glyph */
	for (i = 0; i<count; ++i)
	{
		FT_Int32 flags = 0;
		int ft_glyph_top = 0;
		int ft_glyph_left = 0;
		glyph_index = FT_Get_Char_Index(face, charcodes[i]);
		// WARNING: We use texture-atlas depth to guess if user wants
		//          LCD subpixel rendering

		if (font->outlineType > 0)
		{
			flags |= FT_LOAD_NO_BITMAP;
		}
		else
		{
			flags |= FT_LOAD_RENDER;
		}

		if (!font->hinting)
		{
			flags |= FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT;
		}
		else
		{
			flags |= FT_LOAD_FORCE_AUTOHINT;
		}


		if (depth > 2)
		{
			FT_Library_SetLcdFilter(library, FT_LCD_FILTER_LIGHT);
			flags |= FT_LOAD_TARGET_LCD;
			if (font->filtering)
			{
				FT_Library_SetLcdFilterWeights(library, font->lcdWeights);
			}
		}
		error = FT_Load_Glyph(face, glyph_index, flags);
		if (error)
		{
			FT_Done_Face(face);
			FT_Done_FreeType(library);
			return count - i;
		}


		if (font->outlineType == 0)
		{
			slot = face->glyph;
			ft_bitmap = slot->bitmap;
			ft_glyph_top = slot->bitmap_top;
			ft_glyph_left = slot->bitmap_left;
		}
		else
		{
			FT_Stroker stroker;
			FT_BitmapGlyph ft_bitmap_glyph;
			error = FT_Stroker_New(library, &stroker);
			if (error)
			{
				FT_Done_Face(face);
				FT_Stroker_Done(stroker);
				FT_Done_FreeType(library);
				return 0;
			}
			FT_Stroker_Set(stroker,
				(int)(font->outlineThickness * HRES),
				FT_STROKER_LINECAP_ROUND,
				FT_STROKER_LINEJOIN_ROUND,
				0);
			error = FT_Get_Glyph(face->glyph, &ft_glyph);
			if (error)
			{
				FT_Done_Face(face);
				FT_Stroker_Done(stroker);
				FT_Done_FreeType(library);
				return 0;
			}

			if (font->outlineType == 1)
			{
				error = FT_Glyph_Stroke(&ft_glyph, stroker, 1);
			}
			else if (font->outlineType == 2)
			{
				error = FT_Glyph_StrokeBorder(&ft_glyph, stroker, 0, 1);
			}
			else if (font->outlineType == 3)
			{
				error = FT_Glyph_StrokeBorder(&ft_glyph, stroker, 1, 1);
			}
			if (error)
			{
				FT_Done_Face(face);
				FT_Stroker_Done(stroker);
				FT_Done_FreeType(library);
				return 0;
			}

			if (depth == 1)
			{
				error = FT_Glyph_To_Bitmap(&ft_glyph, FT_RENDER_MODE_NORMAL, 0, 1);
				if (error)
				{
					FT_Done_Face(face);
					FT_Stroker_Done(stroker);
					FT_Done_FreeType(library);
					return 0;
				}
			}
			else
			{
				error = FT_Glyph_To_Bitmap(&ft_glyph, FT_RENDER_MODE_LCD, 0, 1);
				if (error)
				{
					FT_Done_Face(face);
					FT_Stroker_Done(stroker);
					FT_Done_FreeType(library);
					return 0;
				}
			}
			ft_bitmap_glyph = (FT_BitmapGlyph)ft_glyph;
			ft_bitmap = ft_bitmap_glyph->bitmap;
			ft_glyph_top = ft_bitmap_glyph->top;
			ft_glyph_left = ft_bitmap_glyph->left;
			FT_Stroker_Done(stroker);
		}


		// We want each glyph to be separated by at least one black pixel
		// (for example for shader used in demo-subpixel.c)
		w = ft_bitmap.width / depth + 1;
		h = ft_bitmap.rows + 1;
		region = OPfontAtlasGetRegion(font->atlas, w, h);
		if (region.x < 0)
		{
			missed++;
			fprintf(stderr, "Texture atlas is full (line %d)\n", __LINE__);
			continue;
		}
		w = w - 1;
		h = h - 1;
		x = region.x;
		y = region.y;
		OPfontAtlasSetRegion(font->atlas, x, y, w, h,
			ft_bitmap.buffer, ft_bitmap.pitch);

		glyph = OPfontGlyphCreate();
		glyph->charcode = charcodes[i];
		glyph->width = w;
		glyph->height = h;
		glyph->outlineType = font->outlineType;
		glyph->outlineThickness = font->outlineThickness;
		glyph->offsetX = ft_glyph_left;
		glyph->offsetY = ft_glyph_top;
		glyph->textureCoordinates.x = x / (float)width;
		glyph->textureCoordinates.y = y / (float)height;
		glyph->textureCoordinates.z = (x + glyph->width) / (float)width;
		glyph->textureCoordinates.w = (y + glyph->height) / (float)height;

		// Discard hinting to get advance
		FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER | FT_LOAD_NO_HINTING);
		slot = face->glyph;
		glyph->advanceX = slot->advance.x / HRESf;
		glyph->advanceY = slot->advance.y / HRESf;

		OPvectorPush(font->glyphs, (ui8*)&glyph);

		if (font->outlineType > 0)
		{
			FT_Done_Glyph(ft_glyph);
		}
	}
	FT_Done_Face(face);
	FT_Done_FreeType(library);
	//texture_atlas_upload(font->atlas);
	OPfontKerningGenerate(font, filename);
	return missed;
}

OPfont* OPfontFromFile(OPfontAtlas* atlas, OPfloat ptSize, i8* filename)
{
	OPfont* self = (OPfont*)OPalloc(sizeof(OPfont));

	if (!self) {
		return NULL;
	}

	self->atlas = atlas;
	self->size = ptSize;

	if (OPfontInit(self, filename)) {
		OPfontUnload(self);
		return NULL;
	}

	return self;
}

OPint OPfontLoadGlyphs(OPfont* font, i8* filename, i8* charcodes)
{
	OPuint i;
	OPint x, y, width, height, depth, w, h;
	FT_Library library;
	FT_Error error;
	FT_Face face;
	FT_Glyph ft_glyph;
	FT_GlyphSlot slot;
	FT_Bitmap ft_bitmap;

	FT_UInt glyph_index;
	OPfontGlyph* glyph;
	OPfontAtlasRegion region;
	size_t missed = 0;

	width = font->atlas->width;
	height = font->atlas->height;
	depth = font->atlas->depth;

	if (!OPfontGetFace(font, filename, &library, &face))
		return strlen(charcodes);

	/* Load each glyph */
	for (i = 0; i<strlen(charcodes); ++i)
	{
		FT_Int32 flags = 0;
		int ft_glyph_top = 0;
		int ft_glyph_left = 0;
		glyph_index = FT_Get_Char_Index(face, charcodes[i]);
		// WARNING: We use texture-atlas depth to guess if user wants
		//          LCD subpixel rendering

		if (font->outlineType > 0)
		{
			flags |= FT_LOAD_NO_BITMAP;
		}
		else
		{
			flags |= FT_LOAD_RENDER;
		}

		if (!font->hinting)
		{
			flags |= FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT;
		}
		else
		{
			flags |= FT_LOAD_FORCE_AUTOHINT;
		}


		if (depth > 2)
		{
			FT_Library_SetLcdFilter(library, FT_LCD_FILTER_LIGHT);
			flags |= FT_LOAD_TARGET_LCD;
			if (font->filtering)
			{
				FT_Library_SetLcdFilterWeights(library, font->lcdWeights);
			}
		}
		error = FT_Load_Glyph(face, glyph_index, flags);
		if (error)
		{
			FT_Done_Face(face);
			FT_Done_FreeType(library);
			return strlen(charcodes) - i;
		}


		if (font->outlineType == 0)
		{
			slot = face->glyph;
			ft_bitmap = slot->bitmap;
			ft_glyph_top = slot->bitmap_top;
			ft_glyph_left = slot->bitmap_left;
		}
		else
		{
			FT_Stroker stroker;
			FT_BitmapGlyph ft_bitmap_glyph;
			error = FT_Stroker_New(library, &stroker);
			if (error)
			{
				FT_Done_Face(face);
				FT_Stroker_Done(stroker);
				FT_Done_FreeType(library);
				return 0;
			}
			FT_Stroker_Set(stroker,
				(int)(font->outlineThickness * HRES),
				FT_STROKER_LINECAP_ROUND,
				FT_STROKER_LINEJOIN_ROUND,
				0);
			error = FT_Get_Glyph(face->glyph, &ft_glyph);
			if (error)
			{
				FT_Done_Face(face);
				FT_Stroker_Done(stroker);
				FT_Done_FreeType(library);
				return 0;
			}

			if (font->outlineType == 1)
			{
				error = FT_Glyph_Stroke(&ft_glyph, stroker, 1);
			}
			else if (font->outlineType == 2)
			{
				error = FT_Glyph_StrokeBorder(&ft_glyph, stroker, 0, 1);
			}
			else if (font->outlineType == 3)
			{
				error = FT_Glyph_StrokeBorder(&ft_glyph, stroker, 1, 1);
			}
			if (error)
			{
				FT_Done_Face(face);
				FT_Stroker_Done(stroker);
				FT_Done_FreeType(library);
				return 0;
			}

			if (depth == 1)
			{
				error = FT_Glyph_To_Bitmap(&ft_glyph, FT_RENDER_MODE_NORMAL, 0, 1);
				if (error)
				{
					FT_Done_Face(face);
					FT_Stroker_Done(stroker);
					FT_Done_FreeType(library);
					return 0;
				}
			}
			else
			{
				error = FT_Glyph_To_Bitmap(&ft_glyph, FT_RENDER_MODE_LCD, 0, 1);
				if (error)
				{
					FT_Done_Face(face);
					FT_Stroker_Done(stroker);
					FT_Done_FreeType(library);
					return 0;
				}
			}
			ft_bitmap_glyph = (FT_BitmapGlyph)ft_glyph;
			ft_bitmap = ft_bitmap_glyph->bitmap;
			ft_glyph_top = ft_bitmap_glyph->top;
			ft_glyph_left = ft_bitmap_glyph->left;
			FT_Stroker_Done(stroker);
		}


		// We want each glyph to be separated by at least one black pixel
		// (for example for shader used in demo-subpixel.c)
		w = ft_bitmap.width / depth + 1;
		h = ft_bitmap.rows + 1;
		region = OPfontAtlasGetRegion(font->atlas, w, h);
		if (region.x < 0)
		{
			missed++;
			fprintf(stderr, "Texture atlas is full (line %d)\n", __LINE__);
			continue;
		}
		w = w - 1;
		h = h - 1;
		x = region.x;
		y = region.y;
		OPfontAtlasSetRegion(font->atlas, x, y, w, h,
			ft_bitmap.buffer, ft_bitmap.pitch);

		glyph = OPfontGlyphCreate();
		glyph->charcode = charcodes[i];
		glyph->width = w;
		glyph->height = h;
		glyph->outlineType = font->outlineType;
		glyph->outlineThickness = font->outlineThickness;
		glyph->offsetX = ft_glyph_left;
		glyph->offsetY = ft_glyph_top;
		glyph->textureCoordinates.x = x / (float)width;
		glyph->textureCoordinates.y = y / (float)height;
		glyph->textureCoordinates.z = (x + glyph->width) / (float)width;
		glyph->textureCoordinates.w = (y + glyph->height) / (float)height;

		// Discard hinting to get advance
		FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER | FT_LOAD_NO_HINTING);
		slot = face->glyph;
		glyph->advanceX = slot->advance.x / HRESf;
		glyph->advanceY = slot->advance.y / HRESf;

		OPvectorPush(font->glyphs, (ui8*)&glyph);

		if (font->outlineType > 0)
		{
			FT_Done_Glyph(ft_glyph);
		}
	}
	FT_Done_Face(face);
	FT_Done_FreeType(library);
	//texture_atlas_upload(font->atlas);
	OPfontKerningGenerate(font, filename);
	return missed;
}

void OPfontAtlasSave(OPfontAtlas* atlas, i8* filename) {
	if (atlas->depth == 1) {
		ui8* data = (ui8*)OPalloc(atlas->width * atlas->height * 3);
		for (OPint i = atlas->width * atlas->height; i--;) {
			data[i * 3] = atlas->data[i];
			data[i * 3 + 1] = atlas->data[i];
			data[i * 3 + 2] = atlas->data[i];
		}
		OPimagePNGCreate24(data, atlas->width, atlas->height, filename);
	}
	else if (atlas->depth == 3) {
		OPimagePNGCreate24(atlas->data, atlas->width, atlas->height, filename);
	}
	else if (atlas->depth == 4){
		OPimagePNGCreate32(atlas->data, atlas->width, atlas->height, filename);
	}
}

void print_help()
{
	fprintf(stderr, "Usage: FontMaker [--help] --font <font file> --size <font size> --atlas <atlas image size 128, 256, 512, 1024>\n");
}

#include <iostream>
#include <fstream>
using namespace std;

void write(ofstream* stream, void* data, i32 size) {
	stream->write((char*)data, size);
}

void writeI8(ofstream* stream, i8 val) {
	write(stream, &val, sizeof(i8));
}
void writeF32(ofstream* stream, f32 val) {
	write(stream, &val, sizeof(f32));
}
void writeI16(ofstream* stream, i16 val) {
	write(stream, &val, sizeof(i16));
}
void writeI32(ofstream* stream, i32 val) {
	write(stream, &val, sizeof(i32));
}

OPfont* OPfontLoadFile(i8* filename) {
	OPfont* font = (OPfont*)OPalloc(sizeof(OPfont));

	i16 version;

	ifstream myFile(filename, ios::binary);
	char buffer[32];
	myFile.read(buffer, sizeof(i16));		version = *(i16*)buffer;
	myFile.read(buffer, sizeof(f32));		font->size = *(f32*)buffer;
	myFile.read(buffer, sizeof(i32));		font->hinting = *(i32*)buffer;
	myFile.read(buffer, sizeof(i32));		font->outlineType = *(i32*)buffer;
	myFile.read(buffer, sizeof(f32));		font->outlineThickness = *(f32*)buffer;
	myFile.read(buffer, sizeof(i32));		font->filtering = *(i32*)buffer;
	myFile.read(buffer, sizeof(i8));		font->lcdWeights[0] = *(i8*)buffer;
	myFile.read(buffer, sizeof(i8));		font->lcdWeights[1] = *(i8*)buffer;
	myFile.read(buffer, sizeof(i8));		font->lcdWeights[2] = *(i8*)buffer;
	myFile.read(buffer, sizeof(i8));		font->lcdWeights[3] = *(i8*)buffer;
	myFile.read(buffer, sizeof(i8));		font->lcdWeights[4] = *(i8*)buffer;
	myFile.read(buffer, sizeof(i32));		font->kerning = *(i32*)buffer;
	myFile.read(buffer, sizeof(f32));		font->height = *(f32*)buffer;
	myFile.read(buffer, sizeof(f32));		font->lineGap = *(f32*)buffer;
	myFile.read(buffer, sizeof(f32));		font->ascender = *(f32*)buffer;
	myFile.read(buffer, sizeof(f32));		font->descender = *(f32*)buffer;
	myFile.read(buffer, sizeof(f32));		font->underlinePosition = *(f32*)buffer;
	myFile.read(buffer, sizeof(f32));		font->underlineThickness = *(f32*)buffer;
	
	i16 glyphCount;
	myFile.read(buffer, sizeof(i16));		glyphCount = *(i16*)buffer;
	font->glyphs = OPvectorCreate(sizeof(OPfontGlyph));

	for (i16 i = glyphCount; i--;) {
		OPfontGlyph glyph;

		myFile.read(buffer, sizeof(i8));		glyph.charcode = *(i8*)buffer;
		myFile.read(buffer, sizeof(i32));		glyph.width = *(i32*)buffer;
		myFile.read(buffer, sizeof(i32));		glyph.height = *(i32*)buffer;
		myFile.read(buffer, sizeof(i32));		glyph.offsetX = *(i32*)buffer;
		myFile.read(buffer, sizeof(i32));		glyph.offsetY = *(i32*)buffer;
		myFile.read(buffer, sizeof(f32));		glyph.advanceX = *(f32*)buffer;
		myFile.read(buffer, sizeof(f32));		glyph.advanceY = *(f32*)buffer;
		myFile.read(buffer, sizeof(f32));		glyph.textureCoordinates.x = *(f32*)buffer;
		myFile.read(buffer, sizeof(f32));		glyph.textureCoordinates.y = *(f32*)buffer;
		myFile.read(buffer, sizeof(f32));		glyph.textureCoordinates.z = *(f32*)buffer;
		myFile.read(buffer, sizeof(f32));		glyph.textureCoordinates.w = *(f32*)buffer;

		i16 kerningCount;
		myFile.read(buffer, sizeof(i16));		kerningCount = *(i16*)buffer;
		glyph.kerning = OPvectorCreate(sizeof(OPfontKerning));
		for (i16 j = kerningCount; j--;) {
			OPfontKerning kerning;

			myFile.read(buffer, sizeof(i8));		kerning.charcode = *(i8*)buffer;
			myFile.read(buffer, sizeof(f32));		kerning.kerning = *(f32*)buffer;
			OPvectorPush(glyph.kerning, (ui8*)&kerning);
		}

		myFile.read(buffer, sizeof(i32));		glyph.outlineType = *(i32*)buffer;
		myFile.read(buffer, sizeof(f32));		glyph.outlineThickness = *(f32*)buffer;
		
		OPvectorPush(font->glyphs, (ui8*)&glyph);
	}

	return font;
}

void OPfontSave(OPfont* font, i8* filename) {
	ofstream myFile(filename, ios::binary);
	
	// Header
	writeI16(&myFile, 1);					// Version

	// Font Description
	writeF32(&myFile, font->size);			// size;	
	writeI32(&myFile, font->hinting);		// hinting;
	writeI32(&myFile, font->outlineType);	// outlineType;
	writeF32(&myFile, font->outlineThickness);	// outlineThickness;
	writeI32(&myFile, font->filtering);		// filtering;	
	writeI8(&myFile, font->lcdWeights[0]);	// lcdWeights[5];
	writeI8(&myFile, font->lcdWeights[1]);
	writeI8(&myFile, font->lcdWeights[2]);
	writeI8(&myFile, font->lcdWeights[3]);
	writeI8(&myFile, font->lcdWeights[4]);
	writeI32(&myFile, font->kerning);		// kerning;
	writeF32(&myFile, font->height);		// height;
	writeF32(&myFile, font->lineGap);		// lineGap;
	writeF32(&myFile, font->ascender);		// ascender;
	writeF32(&myFile, font->descender);		// descender;
	writeF32(&myFile, font->underlinePosition);	// underlinePosition;
	writeF32(&myFile, font->underlineThickness);	// underlineThickness;

	// Font Glyphs
	writeI16(&myFile, font->glyphs->_size);
	for (i32 i = font->glyphs->_size; i--;) {
		OPfontGlyph* glyph = *(OPfontGlyph**)OPvectorGet(font->glyphs, i);

		writeI8(&myFile, glyph->charcode);
		writeI32(&myFile, glyph->width);
		writeI32(&myFile, glyph->height);
		writeI32(&myFile, glyph->offsetX);
		writeI32(&myFile, glyph->offsetY);
		writeF32(&myFile, glyph->advanceX);
		writeF32(&myFile, glyph->advanceY);

		writeF32(&myFile, glyph->textureCoordinates.x);
		writeF32(&myFile, glyph->textureCoordinates.y);
		writeF32(&myFile, glyph->textureCoordinates.z);
		writeF32(&myFile, glyph->textureCoordinates.w);

		writeI16(&myFile, glyph->kerning->_size);
		for (i32 j = glyph->kerning->_size; j--;) {
			OPfontKerning* kerning = (OPfontKerning*)OPvectorGet(glyph->kerning, j);
			writeI8(&myFile, kerning->charcode);
			writeF32(&myFile, kerning->kerning);
		}

		writeI32(&myFile, glyph->outlineType);
		writeF32(&myFile, glyph->outlineThickness);
	}

	ui8* data;
	OPuint dataSize;
	if (font->atlas->depth == 1) {
		ui8* imageData = (ui8*)OPalloc(font->atlas->width * font->atlas->height * 3);
		for (OPint i = font->atlas->width * font->atlas->height; i--;) {
			imageData[i * 3] = font->atlas->data[i];
			imageData[i * 3 + 1] = font->atlas->data[i];
			imageData[i * 3 + 2] = font->atlas->data[i];
		}
		OPimagePNG24WriteStream(imageData, font->atlas->width, font->atlas->height, &data, &dataSize);
	}
	else if (font->atlas->depth == 3) {
		OPimagePNG24WriteStream(font->atlas->data, font->atlas->width, font->atlas->height, &data, &dataSize);
	}
	else if (font->atlas->depth == 4){
		OPimagePNG32WriteStream(font->atlas->data, font->atlas->width, font->atlas->height, &data, &dataSize);
	}

	write(&myFile, data, dataSize);

	myFile.close();
}

int main(int argc, char **argv) {
	//OPfont* testFont = OPfontLoadFile("output.opf");

	int arg;

	i8* font_cache = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	OPfloat font_size = 0.0;
	OPint atlasSize = 0;
	i8* font_filename = NULL;

	for (arg = 1; arg < argc; ++arg)
	{
		if (0 == strcmp("--help", argv[arg]) || 0 == strcmp("-h", argv[arg]))
		{
			print_help();
			exit(1);
		}

		if (0 == strcmp("--font", argv[arg]) || 0 == strcmp("-f", argv[arg]))
		{
			++arg;

			if (font_filename)
			{
				fprintf(stderr, "Multiple --font parameters.\n");
				print_help();
				exit(1);
			}

			if (arg >= argc)
			{
				fprintf(stderr, "No font file given.\n");
				print_help();
				exit(1);
			}

			font_filename = argv[arg];
			continue;
		}

		if (0 == strcmp("--size", argv[arg]) || 0 == strcmp("-s", argv[arg]))
		{
			++arg;

			if (0.0 != font_size)
			{
				fprintf(stderr, "Multiple --size parameters.\n");
				print_help();
				exit(1);
			}

			if (arg >= argc)
			{
				fprintf(stderr, "No font size given.\n");
				print_help();
				exit(1);
			}

			errno = 0;

			font_size = (OPfloat)atof(argv[arg]);

			if (errno)
			{
				fprintf(stderr, "No valid font size given.\n");
				print_help();
				exit(1);
			}

			continue;
		}

		if (0 == strcmp("--atlas", argv[arg]) || 0 == strcmp("-a", argv[arg]))
		{
			++arg;

			if (0.0 != atlasSize)
			{
				fprintf(stderr, "Multiple --atlas parameters.\n");
				print_help();
				exit(1);
			}

			if (arg >= argc)
			{
				fprintf(stderr, "No atlas size given.\n");
				print_help();
				exit(1);
			}

			errno = 0;

			atlasSize = atoi(argv[arg]);

			if (errno)
			{
				fprintf(stderr, "No valid atlas size given.\n");
				print_help();
				exit(1);
			}

			continue;
		}
	}

	if (font_size == 0.0) {
		fprintf(stderr, "No font size was given.\n");
		print_help();
		exit(1);
	}

	if (atlasSize == 0.0) {
		fprintf(stderr, "No atlas size was given.\n");
		print_help();
		exit(1);
	}


	OPfontAtlas* atlas = OPfontAtlasCreate(atlasSize, atlasSize, 1);
	OPfont* font = OPfontFromFile(atlas, font_size, font_filename);
	OPfontLoadGlyphs(font, font_filename, font_cache);
	OPfontAtlasSave(atlas, "output.png");
	OPfontSave(font, "output.opf");

	return 0;
}