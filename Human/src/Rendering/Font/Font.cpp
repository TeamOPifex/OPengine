#include "./Human/include/Rendering/Font/Font.h"
#include "./Human/include/Rendering/Font/FontAtlas.h"
#include "./Human/include/Rendering/Font/FontGlyph.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Utilities/ImagePNG.h"
#include "./Data/include/Stream.h"
#include "./Data/include/File.h"
#include "./Core/include/Assert.h"
#include "./Core/include/MathHelpers.h"

void OPfontLoad(OPchar* filename, OPfont** data) {
	OPfont* font = (OPfont*)OPalloc(sizeof(OPfont));
	*data = font;

	i16 version;
	OPstream* str = OPreadFile(filename);

	version = OPreadi16(str);
	font->size = OPreadf32(str);
	font->hinting = OPreadi32(str);
	font->outlineType = OPreadi32(str);
	font->outlineThickness = OPreadf32(str);
	font->filtering = OPreadi32(str);
	font->lcdWeights[0] = OPreadi8(str);
	font->lcdWeights[1] = OPreadi8(str);
	font->lcdWeights[2] = OPreadi8(str);
	font->lcdWeights[3] = OPreadi8(str);
	font->lcdWeights[4] = OPreadi8(str);
	font->kerning = OPreadi32(str);
	font->height = OPreadf32(str);
	font->lineGap = OPreadf32(str);
	font->ascender = OPreadf32(str);
	font->descender = OPreadf32(str);
	font->underlinePosition = OPreadf32(str);
	font->underlineThickness = OPreadf32(str);

	i16 glyphCount;
	glyphCount = OPreadi16(str);
	font->glyphs = OPvectorCreate(sizeof(OPfontGlyph));

	for (i16 i = glyphCount; i--;) {
		OPfontGlyph* glyph = (OPfontGlyph*)OPalloc(sizeof(OPfontGlyph));

		glyph->charcode = OPreadi8(str);
		glyph->width = OPreadi32(str);
		glyph->height = OPreadi32(str);
		glyph->offsetX = OPreadi32(str);
		glyph->offsetY = OPreadi32(str);
		glyph->advanceX = OPreadf32(str);
		glyph->advanceY = OPreadf32(str);
		glyph->textureCoordinates.x = OPreadf32(str);
		glyph->textureCoordinates.y = OPreadf32(str);
		glyph->textureCoordinates.z = OPreadf32(str);
		glyph->textureCoordinates.w = OPreadf32(str);

		i16 kerningCount;
		kerningCount = OPreadi16(str);
		glyph->kerning = OPvectorCreate(sizeof(OPfontKerning));
		for (i16 j = kerningCount; j--;) {
			OPfontKerning kerning;

			kerning.charcode = OPreadi8(str);
			kerning.kerning = OPreadf32(str);
			OPvectorPush(glyph->kerning, (ui8*)&kerning);
		}

		glyph->outlineType = OPreadi32(str);
		glyph->outlineThickness = OPreadf32(str);

		OPvectorPush(font->glyphs, (ui8*)&glyph);
	}

	OPimagePNGLoadStream(str, str->_pointer, &font->texture); 
}

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

OPfontGlyph* OPfontGetGlyph(OPfont* font, OPchar charcode)
{
	size_t i;
	OPchar buffer[2] = { 0, 0 };
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
	if (charcode == (OPchar)(-1))
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
		glyph->charcode = (OPchar)(-1);
		glyph->textureCoordinates.x = (region.x + 2) / (float)width;
		glyph->textureCoordinates.y = (region.y + 2) / (float)height;
		glyph->textureCoordinates.z = (region.x + 3) / (float)width;
		glyph->textureCoordinates.w = (region.y + 3) / (float)height;
		OPvectorPush(font->glyphs, (ui8*)&glyph);
		return glyph;
	}

	return NULL;
}


OPmesh OPfontCreateText(OPfont* font, OPchar* text) {
	ui32 vertexSize = sizeof(OPvertexColor);
	ui32 indexSize = sizeof(ui16);
	OPvector* vertices = OPvectorCreate(vertexSize);
	OPvector* indices = OPvectorCreate(indexSize);

	size_t i;

	OPfloat width = 0;
	for (i = 0; i< strlen(text); ++i)
	{
		OPfontGlyph* glyph = OPfontGetGlyph(font, text[i]);
		if (glyph != NULL)
		{
			OPfloat kerning = 0;
			if (i > 0)
			{
				kerning = OPfontGlyphGetKerning(glyph, text[i - 1]);
			}
			width += kerning;
			int x0 = (int)(width + glyph->offsetX);
			int y0 = (int)(glyph->offsetY);
			int x1 = (int)(x0 + glyph->width);
			int y1 = (int)(y0 - glyph->height);
			float s0 = glyph->textureCoordinates.x;
			float t0 = glyph->textureCoordinates.y;
			float s1 = glyph->textureCoordinates.z;
			float t1 = glyph->textureCoordinates.w;

			OPint offset = vertices->_size;
			ui16 inds[6] = { 0 + offset, 1 + offset, 2 + offset, 0 + offset, 2 + offset, 3 + offset };
			OPvertexColor verts[4] = { { (OPfloat)x0, (OPfloat)y0, 0.0f, s0, t0 },
			{ (OPfloat)x0, (OPfloat)y1, 0.0f, s0, t1 },
			{ (OPfloat)x1, (OPfloat)y1, 0.0f, s1, t1 },
			{ (OPfloat)x1, (OPfloat)y0, 0.0f, s1, t0 } };

			for (OPint i = 0; i < 4; i++)
				OPvectorPush(vertices, (ui8*)&verts[i]);
			for (OPint i = 0; i < 6; i++)
				OPvectorPush(indices, (ui8*)&inds[i]);
			
			width += glyph->advanceX;
		}
	}

	OPmesh mesh = OPrenderCreateMesh();
	OPrenderBindMesh(&mesh);
	OPrenderBuildMesh(vertexSize, indexSize, vertices->_size, indices->_size, vertices->items, indices->items);
	return mesh;	
}

OPfontBuiltTextNode OPfontCreatePackedText(OPfont* font, const OPchar* text) {
	ASSERT(OPRENDER_CURR_PACKER != NULL, "No mesh packer bound.");

	ui32 vertexSize = sizeof(OPvertexColor);
	ui32 indexSize = sizeof(ui16);
	OPvector* vertices = OPvectorCreate(vertexSize);
	OPvector* indices = OPvectorCreate(indexSize);

	size_t i;

	OPfloat width = 0;
	for (i = 0; i< strlen(text); ++i)
	{
		OPfontGlyph* glyph = OPfontGetGlyph(font, text[i]);
		if (glyph != NULL)
		{
			OPfloat kerning = 0;
			if (i > 0)
			{
				kerning = OPfontGlyphGetKerning(glyph, text[i - 1]);
			}
			width += kerning;
			int x0 = (int)(width + glyph->offsetX);
			int y0 = (int)(glyph->offsetY);
			int x1 = (int)(x0 + glyph->width);
			int y1 = (int)(y0 - glyph->height);
			float s0 = glyph->textureCoordinates.x;
			float t0 = glyph->textureCoordinates.y;
			float s1 = glyph->textureCoordinates.z;
			float t1 = glyph->textureCoordinates.w;

			OPint offset = vertices->_size;
			ui16 inds[6] = { 0 + offset, 1 + offset, 2 + offset, 0 + offset, 2 + offset, 3 + offset };
			OPvertexColor verts[4] = { { (OPfloat)x0, (OPfloat)y0, 0.0f, s0, t0 },
			{ (OPfloat)x0, (OPfloat)y1, 0.0f, s0, t1 },
			{ (OPfloat)x1, (OPfloat)y1, 0.0f, s1, t1 },
			{ (OPfloat)x1, (OPfloat)y0, 0.0f, s1, t0 } };

			for (OPint i = 0; i < 4; i++)
				OPvectorPush(vertices, (ui8*)&verts[i]);
			for (OPint i = 0; i < 6; i++)
				OPvectorPush(indices, (ui8*)&inds[i]);

			width += glyph->advanceX;
		}
	}

	OPfontBuiltTextNode node;
	node.Width = width;
	node.packedMesh = (OPmeshPacked*)OPalloc(sizeof(OPmeshPacked));
	*node.packedMesh = OPrenderCreateMeshPacked(vertexSize, indexSize, vertices->_size, indices->_size, vertices->items, indices->items);
	
	return node;
}
