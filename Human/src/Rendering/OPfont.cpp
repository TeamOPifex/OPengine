#pragma once

#include "include\Rendering\OPfont.h"
#include "./include/Rendering/OPfontAtlas.h"
#include "./include/Rendering/OPfontGlyph.h"
#include "./Core/include/MathHelpers.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Data/include/OPstream.h"
#include "./Data/include/OPfile.h"
#include "./Human/include/Utilities/ImagePNG.h"

void OPfontLoad(i8* filename, OPfont** data) {
	OPfont* font = (OPfont*)OPalloc(sizeof(OPfont));
	*data = font;

	i16 version;
	OPstream* str = OPreadFile(filename);

	version = OPread_i16(str);
	font->size = OPread_f32(str);
	font->hinting = OPread_i32(str);
	font->outlineType = OPread_i32(str);
	font->outlineThickness = OPread_f32(str);
	font->filtering = OPread_i32(str);
	font->lcdWeights[0] = OPread_i8(str);
	font->lcdWeights[1] = OPread_i8(str);
	font->lcdWeights[2] = OPread_i8(str);
	font->lcdWeights[3] = OPread_i8(str);
	font->lcdWeights[4] = OPread_i8(str);
	font->kerning = OPread_i32(str);
	font->height = OPread_f32(str);
	font->lineGap = OPread_f32(str);
	font->ascender = OPread_f32(str);
	font->descender = OPread_f32(str);
	font->underlinePosition = OPread_f32(str);
	font->underlineThickness = OPread_f32(str);

	i16 glyphCount;
	glyphCount = OPread_i16(str);
	font->glyphs = OPvectorCreate(sizeof(OPfontGlyph));

	for (i16 i = glyphCount; i--;) {
		OPfontGlyph* glyph = (OPfontGlyph*)OPalloc(sizeof(OPfontGlyph));

		glyph->charcode = OPread_i8(str);
		glyph->width = OPread_i32(str);
		glyph->height = OPread_i32(str);
		glyph->offsetX = OPread_i32(str);
		glyph->offsetY = OPread_i32(str);
		glyph->advanceX = OPread_f32(str);
		glyph->advanceY = OPread_f32(str);
		glyph->textureCoordinates.x = OPread_f32(str);
		glyph->textureCoordinates.y = OPread_f32(str);
		glyph->textureCoordinates.z = OPread_f32(str);
		glyph->textureCoordinates.w = OPread_f32(str);

		i16 kerningCount;
		kerningCount = OPread_i16(str);
		glyph->kerning = OPvectorCreate(sizeof(OPfontKerning));
		for (i16 j = kerningCount; j--;) {
			OPfontKerning kerning;

			kerning.charcode = OPread_i8(str);
			kerning.kerning = OPread_f32(str);
			OPvectorPush(glyph->kerning, (ui8*)&kerning);
		}

		glyph->outlineType = OPread_i32(str);
		glyph->outlineThickness = OPread_f32(str);

		OPvectorPush(font->glyphs, (ui8*)&glyph);
	}

	OPimagePNGLoadStream(str, str->_pointer, &font->texture); 
}

//OPfont* OPfontFromFile(OPfontAtlas* atlas, OPfloat ptSize, i8* filename)
//{
//	OPfont* self = (OPfont*)OPalloc(sizeof(OPfont));
//
//	if (!self) {
//		return NULL;
//	}
//
//	self->atlas = atlas;
//	self->size = ptSize;
//
//	if (OPfontInit(self)) {
//		OPfontDestroy(self);
//		return NULL;
//	}
//
//	return self;
//}

void OPfontUnload(OPfont* font)
{
	OPint i;
	OPfontGlyph* glyph;

	for (i = 0; i<OPvectorSize(font->glyphs); ++i)
	{
		glyph = *(OPfontGlyph**)OPvectorGet(font->glyphs, i);
		OPfontGlyphDestroy(glyph);
	}

	OPvectorDestroy(font->glyphs);
	OPimagePNGUnload(font->texture);
	OPfree(font);
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
	//buffer[0] = charcode;
	//if (OPfontLoadGlyphs(font, buffer, 1) == 0)
	//{
	//	return *(OPfontGlyph **)OPvectorBack(font->glyphs);
	//}
	return NULL;
}


OPmesh OPfontCreateText(OPfont* font, i8* text, OPvec4* color, OPvec2* pos, OPint align) {
	ui32 vertexSize = sizeof(OPvertexColor);
	ui32 indexSize = sizeof(ui16);
	OPvector* vertices = OPvectorCreate(vertexSize);
	OPvector* indices = OPvectorCreate(indexSize);

	size_t i;
	float r = color->x, g = color->y, b = color->z, a = color->w;
	OPfloat width = 0;
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
			width += kerning;
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
			width += glyph->advanceX;
		}
	}

	if (align == 1) {
		for (ui32 i = 0; i < vertices->_size; i++) {
			OPvertexColor* vert = (OPvertexColor*)OPvectorGet(vertices, i);
			vert->Position.x -= width / 2.0f;
		}
	}

	OPmesh mesh = OPrenderCreateMesh();
	OPrenderBindMesh(&mesh);
	OPrenderBuildMesh(vertexSize, indexSize, vertices->_size, indices->_size, vertices->items, indices->items);
	return mesh;	
}