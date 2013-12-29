#pragma once

#include "include\Rendering\OPfont.h"
#include "./include/Rendering/OPfontAtlas.h"
#include "./include/Rendering/OPfontHelpers.h"
#include "./include/Rendering/OPfontGlyph.h"
#include "./Core/include/MathHelpers.h"
#include "./Human/include/Rendering/OPMvertex.h"

OPfont* OPfontFromFile(OPfontAtlas* atlas, OPfloat ptSize, i8* filename)
{
	OPfont* self = (OPfont*)OPalloc(sizeof(OPfont));

	if (!self) {
		return NULL;
	}

	self->atlas = atlas;
	self->size = ptSize;

	self->filename = filename;

	if (OPfontInit(self)) {
		OPfontDestroy(self);
		return NULL;
	}

	return self;
}

void OPfontDestroy(OPfont* font)
{
	OPint i;
	OPfontGlyph* glyph;
	OPfree(font->filename);

	for (i = 0; i<OPvectorSize(font->glyphs); ++i)
	{
		glyph = *(OPfontGlyph**)OPvectorGet(font->glyphs, i);
		OPfontGlyphDestroy(glyph);
	}

	OPvectorDestroy(font->glyphs);
	free(font);
}

static OPint OPfontInit(OPfont* font)
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
	if (!OPfontGetHiResFace(font, &library, &face))
		return -1;

	font->underlinePosition = face->underline_position / (float)(HRESf*HRESf) * font->size;
	font->underlinePosition = OPround(font->underlinePosition);
	if (font->underlinePosition > -2)
	{
		font->underlinePosition = -2.0;
	}

	font->underlineThickness = face->underline_thickness / (float)(HRESf*HRESf) * font->size;
	font->underlineThickness = round(font->underlineThickness);
	if (font->underlineThickness < 1)
	{
		font->underlineThickness = 1.0;
	}

	metrics = face->size->metrics;
	font->ascender = (metrics.ascender >> 6) / 100.0;
	font->descender = (metrics.descender >> 6) / 100.0;
	font->height = (metrics.height >> 6) / 100.0;
	font->lineGap = font->height - font->ascender + font->descender;
	FT_Done_Face(face);
	FT_Done_FreeType(library);

	/* -1 is a special glyph */
	OPfontGetGlyph(font, -1);

	return 0;
}

OPint OPfontLoadGlyphs(OPfont* font, i8* charcodes, OPint count)
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

	if (!OPfontGetFace(font, &library, &face))
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
	OPfontKerningGenerate(font);
	return missed;
}

OPfontGlyph* OPfontGetGlyph(OPfont* font, i8 charcode)
{
	size_t i;
	i8 buffer[2] = { 0, 0 };
	OPfontGlyph* glyph;

	/* Check if charcode has been already loaded */
	for (i = 0; i<font->glyphs->_size; ++i)
	{
		glyph = *(OPfontGlyph**)OPvectorGet(font->glyphs, i);
		// If charcode is -1, we don't care about outline type or thickness
		if ((glyph->charcode == charcode) &&
			((charcode == (wchar_t)(-1)) ||
			((glyph->outlineType == font->outlineType) &&
			(glyph->outlineThickness == font->outlineThickness))))
		{
			return glyph;
		}
	}

	/* charcode -1 is special : it is used for line drawing (overline,
	* underline, strikethrough) and background.
	*/
	if (charcode == (i8)(-1))
	{
		size_t width = font->atlas->width;
		size_t height = font->atlas->height;
		OPfontAtlasRegion region = OPfontAtlasGetRegion(font->atlas, 5, 5);
		OPfontGlyph * glyph = OPfontGlyphCreate();
		static ui8 data[4 * 4 * 3] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
		if (region.x < 0)
		{
			fprintf(stderr, "Texture atlas is full (line %d)\n", __LINE__);
			return NULL;
		}
		
		OPfontAtlasSetRegion(font->atlas, region.x, region.y, 4, 4, data, 0);
		glyph->charcode = (i8)(-1);
		glyph->textureCoordinates.x = (region.x + 2) / (float)width;
		glyph->textureCoordinates.y = (region.y + 2) / (float)height;
		glyph->textureCoordinates.z = (region.x + 3) / (float)width;
		glyph->textureCoordinates.w = (region.y + 3) / (float)height;
		OPvectorPush(font->glyphs, (ui8*)&glyph);
		return glyph;
	}

	/* Glyph has not been already loaded */
	buffer[0] = charcode;
	if (OPfontLoadGlyphs(font, buffer, 1) == 0)
	{
		return *(OPfontGlyph **)OPvectorBack(font->glyphs);
	}
	return NULL;
}


OPmesh OPfontCreateText(OPfont* font, i8* text, OPvec4* color, OPvec2* pos) {
	ui32 vertexSize = sizeof(OPvertexColor);
	ui32 indexSize = sizeof(ui16);
	OPvector* vertices = OPvectorCreate(vertexSize);
	OPvector* indices = OPvectorCreate(indexSize);

	size_t i;
	float r = color->x, g = color->y, b = color->z, a = color->w;
	for (i = 0; i< strlen(text); ++i)
	{
		OPfontGlyph* glyph = OPfontGetGlyph(font, text[i]);
		if (glyph != NULL)
		{
			OPint kerning = 0;
			if (i > 0)
			{
				kerning = OPfontGlyphGetKerning(glyph, text[i - 1]);
			}
			pos->x += kerning;
			int x0 = (int)(pos->x + glyph->offsetX);
			int y0 = (int)(pos->y + glyph->offsetY);
			int x1 = (int)(x0 + glyph->width);
			int y1 = (int)(y0 - glyph->height);
			float s0 = glyph->textureCoordinates.x;
			float t0 = glyph->textureCoordinates.y;
			float s1 = glyph->textureCoordinates.z;
			float t1 = glyph->textureCoordinates.w;

			OPint offset = vertices->_size;
			ui16 inds[6] = { 0 + offset, 1 + offset, 2 + offset, 0 + offset, 2 + offset, 3 + offset };
			OPvertexColor verts[4] = { { x0, y0, 0, s0, t0 },
			{ x0, y1, 0, s0, t1 },
			{ x1, y1, 0, s1, t1 },
			{ x1, y0, 0, s1, t0 } };

			for (OPint i = 0; i < 4; i++)
				OPvectorPush(vertices, (ui8*)&verts[i]);
			for (OPint i = 0; i < 6; i++)
				OPvectorPush(indices, (ui8*)&inds[i]);
			
			pos->x += glyph->advanceX;
		}
	}

	OPmesh mesh = OPrenderCreateMesh();
	OPrenderBindMesh(&mesh);
	OPrenderBuildMesh(vertexSize, indexSize, vertices->_size, indices->_size, vertices->items, indices->items);
	return mesh;	
}