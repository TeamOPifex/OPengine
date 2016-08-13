#include "./Human/include/Rendering/Font/OPfont.h"
#include "./Human/include/Rendering/Font/OPfontAtlas.h"
#include "./Human/include/Rendering/Font/OPfontGlyph.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Rendering/OPimage.h"
#include "./Human/include/Utilities/OPimagePNG.h"
#include "./Data/include/OPstream.h"
#include "./Data/include/OPfile.h"
#include "./Core/include/Assert.h"
#include "./Core/include/OPmath.h"

OPint OPfontLoad(OPstream* str, OPfont** data) {
	OPfont* font = (OPfont*)OPalloc(sizeof(OPfont));
	*data = font;

	i16 version;
	//OPstream* str = OPreadFile(filename);

	version = str->I16();
	font->size = str->F32();
	font->hinting = str->I32();
	font->outlineType = str->I32();
	font->outlineThickness = str->F32();
	font->filtering = str->I32();
	font->lcdWeights[0] = str->I8();
	font->lcdWeights[1] = str->I8();
	font->lcdWeights[2] = str->I8();
	font->lcdWeights[3] = str->I8();
	font->lcdWeights[4] = str->I8();
	font->kerning = str->I32();
	font->height = str->F32();
	font->lineGap = str->F32();
	font->ascender = str->F32();
	font->descender = str->F32();
	font->underlinePosition = str->F32();
	font->underlineThickness = str->F32();

	i16 glyphCount;
	glyphCount = str->I16();
	font->glyphs = OPvector::Create(sizeof(OPfontGlyph));

	for (i16 i = glyphCount; i--;) {
		OPfontGlyph* glyph = (OPfontGlyph*)OPalloc(sizeof(OPfontGlyph));

		glyph->charcode = str->I8();
		glyph->width = str->I32();
		glyph->height = str->I32();
		glyph->offsetX = str->I32();
		glyph->offsetY = str->I32();
		glyph->advanceX = str->F32();
		glyph->advanceY = str->F32();
		glyph->textureCoordinates.x = str->F32();
		glyph->textureCoordinates.y = str->F32();
		glyph->textureCoordinates.z = str->F32();
		glyph->textureCoordinates.w = str->F32();

		i16 kerningCount;
		kerningCount = str->I16();
		glyph->kerning = OPvector::Create(sizeof(OPfontKerning));
		for (i16 j = kerningCount; j--;) {
			OPfontKerning kerning;

			kerning.charcode = str->I8();
			kerning.kerning = str->F32();
			glyph->kerning->Push((ui8*)&kerning);
		}

		glyph->outlineType = str->I32();
		glyph->outlineThickness = str->F32();

		font->glyphs->Push((ui8*)&glyph);
	}

	OPimagePNGLoadStream(str, str->_pointer, &font->texture);

	return 1;
}

OPint OPfontUnload(OPfont* font)
{
	OPint i;
	OPfontGlyph* glyph;

	for (i = 0; i< font->glyphs->Size(); i++)
	{
		glyph = *(OPfontGlyph**)font->glyphs->Get(i);
		OPfontGlyphDestroy(glyph);
	}

	font->glyphs->Destroy();
	OPfree(font->glyphs);
	OPimagePNGUnload(font->texture);
	OPfree(font);

	return 1;
}

OPfontGlyph* OPfontGetGlyph(OPfont* font, OPchar charcode)
{
	size_t i;
	OPchar buffer[2] = { 0, 0 };
	OPfontGlyph* glyph;

	/* Check if charcode has been already loaded */
	for (i = 0; i<font->glyphs->_size; ++i)
	{
		glyph = *(OPfontGlyph**)font->glyphs->Get(i);
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
		static ui8 data[4 * 4 * 3] = { };
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
		font->glyphs->Push((ui8*)&glyph);
		return glyph;
	}

	return NULL;
}

OPvec2 _OPfontBuild(OPvector* vertices, OPvector* indices, OPfont* font, const OPchar* text, OPfloat scale) {

	OPfloat width = 0;
	OPfloat height = font->height;

	size_t i;

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
			width += kerning * scale;

			int x0 = (int)(width + glyph->offsetX * scale);
			int x1 = (int)(x0 + glyph->width * scale);
			int y0 = (int)(font->size * scale-(int)(glyph->offsetY * scale) - (font->height * scale - font->size * scale));
			int y1 = (int)(font->size * scale-(int)(glyph->offsetY * scale - glyph->height * scale) - (font->height * scale - font->size * scale));

			float s0 = glyph->textureCoordinates.x;
			float t0 = glyph->textureCoordinates.y;
			float s1 = glyph->textureCoordinates.z;
			float t1 = glyph->textureCoordinates.w;

			ui16 offset = (ui16)vertices->_size;
			ui16 inds[6];
			inds[0] = offset; inds[1] = offset + 1; inds[2] = offset + 2;
			inds[3] = offset; inds[4] = offset + 2; inds[5] = offset + 3;
			OPvertexTex verts[4] = { 
				{ OPvec3((f32)x0, (f32)y0, 0.0f), OPvec2(s0, t0) },
				{ OPvec3((f32)x0, (f32)y1, 0.0f), OPvec2(s0, t1) },
				{ OPvec3((f32)x1, (f32)y1, 0.0f), OPvec2(s1, t1) },
				{ OPvec3((f32)x1, (f32)y0, 0.0f), OPvec2(s1, t0) } };

			for (OPint i = 0; i < 4; i++) {
				vertices->Push((ui8*)&verts[i]);
			}
			for (OPint i = 0; i < 6; i++)
				indices->Push((ui8*)&inds[i]);

			width += glyph->advanceX * scale;
		}
	}

	return OPvec2(width, height);
}

OPmesh OPfontCreateText(OPfont* font, OPchar* text) {
	ui32 vertexSize = sizeof(OPvertexTex);
	OPindexSize indexSize = OPindexSize::SHORT;// sizeof(ui16);
	OPvector* vertices = OPvector::Create(vertexSize);
	OPvector* indices = OPvector::Create((ui32)indexSize);

	_OPfontBuild(vertices, indices, font, text, 1);

	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::UV);
	OPvertexLayout vertexLayout = builder.Build();
	OPmesh mesh = OPmesh(vertexLayout);
	mesh.Build(vertexLayout, indexSize, (ui32)vertices->_size, (ui32)indices->_size, vertices->items, indices->items);
	return mesh;
}

OPfontBuiltTextNode OPfontCreatePackedText(OPfont* font, const OPchar* text) {
	return OPfontCreatePackedText(font, text, 1);
}

OPfontBuiltTextNode OPfontCreatePackedText(OPfont* font, const OPchar* text, OPfloat scale) {
	ASSERT(OPMESHPACKER_ACTIVE != NULL, "No mesh packer bound.");

	ui32 vertexSize = sizeof(OPvertexTex);
	OPindexSize indexSize = OPindexSize::SHORT;// sizeof(ui16);
	OPvector* vertices = OPvector::Create(vertexSize);
	OPvector* indices = OPvector::Create((ui32)indexSize);

	OPvec2 size = _OPfontBuild(vertices, indices, font, text, scale);

	OPfontBuiltTextNode node;
	node.Width = size.x;
	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::UV);
	OPvertexLayout vertexLayout = builder.Build();
	node.packedMesh = OPmeshPacked::Create(vertexLayout, indexSize, vertices->_size, indices->_size, vertices->items, indices->items);

	return node;
}

OPvec2 OPfontGetSize(OPfont* font, const OPchar* text, OPfloat scale) {

	size_t i;
	OPfloat width = 0;
	OPfloat height = font->size * scale;
	for (i = 0; i< strlen(text); ++i)
	{
		OPfontGlyph* glyph = OPfontGetGlyph(font, text[i]);
		if (glyph != NULL)
		{
			if (i > 0)
				width += OPfontGlyphGetKerning(glyph, text[i - 1]) * scale;
			width += glyph->advanceX * scale;
		}
	}
	return OPvec2(width, height);
}

OPfontUserTextNode OPfontCreateUserText(OPfont* font, const OPchar* text) {
	return OPfontCreateUserText(font, text, 1);
}

OPfontUserTextNode OPfontCreateUserText(OPfont* font, const OPchar* text, float scale) {

	ui32 vertexSize = sizeof(OPvertexTex);
	ui32 texcoordsSize = sizeof(OPvec2);
	ui32 indexSize = sizeof(ui16);
	OPvector* vertices = OPvector::Create(vertexSize);
	OPvector* indices = OPvector::Create(indexSize);

	OPvec2 size = _OPfontBuild(vertices, indices, font, text, scale);

	OPfontUserTextNode node;
	node.Width = size.x;
	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::UV);
	node.mesh = OPmesh(builder.Build());

	node.mesh.Build(node.mesh.vertexLayout, OPindexSize::SHORT, (ui32)vertices->_size, (ui32)indices->_size, vertices->items, indices->items);

	vertices->Destroy();
	OPfree(vertices);
	indices->Destroy();
	OPfree(indices);

	return node;
}
