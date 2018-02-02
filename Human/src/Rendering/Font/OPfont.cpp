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


void OPfont::Init() {
	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::UV);
	vertexLayout = builder.Build();
	dummyTextNode.mesh = OPmodel(1, vertexLayout);

	ui32 vertexSize = sizeof(OPvertexTex);
	ui32 indexSize = sizeof(ui16);
	vertices = OPvector::Create(vertexSize);
	indices = OPvector::Create(indexSize);
}

void OPfont::Destroy() {
	OPint i;
	OPfontGlyph* glyph;

	for (i = 0; i< glyphCount; i++)
	{
		glyphs[i].Destroy();
	}
	OPfree(glyphs);

	dummyTextNode.mesh.Destroy();

	vertices->Destroy();
	OPfree(vertices);
	indices->Destroy();
	OPfree(indices);
}

OPfontGlyph* OPfont::GetGlyph(OPchar charcode)
{
	size_t i;
	OPchar buffer[2] = { 0, 0 };
	OPfontGlyph* glyph;

	/* Check if charcode has been already loaded */
	for (i = 0; i< glyphCount; ++i)
	{
		glyph = &glyphs[i];// *(OPfontGlyph**)glyphs->Get(i);
		// If charcode is -1, we don't care about outline type or thickness
		if ((glyph->charcode == charcode) &&
			((charcode == (wchar_t)(-1)) ||
			((glyph->outlineType == outlineType) &&
			(glyph->outlineThickness == outlineThickness))))
		{
			return glyph;
		}
	}

	///* charcode -1 is special : it is used for line drawing (overline,
	//* underline, strikethrough) and background.
	//*/
	//if (charcode == (OPchar)(-1))
	//{
	//	size_t width = atlas->width;
	//	size_t height = atlas->height;
	//	OPfontAtlasRegion region = atlas->GetRegion(5, 5);
	//	OPfontGlyph * glyph = OPfontGlyph::Create();
	//	static ui8 data[4 * 4 * 3] = { };
	//	if (region.x < 0)
	//	{
	//		fprintf(stderr, "Texture atlas is full (line %d)\n", __LINE__);
	//		return NULL;
	//	}

	//	atlas->SetRegion(region.x, region.y, 4, 4, data, 0);
	//	glyph->charcode = (OPchar)(-1);
	//	glyph->textureCoordinates.x = (region.x + 2) / (float)width;
	//	glyph->textureCoordinates.y = (region.y + 2) / (float)height;
	//	glyph->textureCoordinates.z = (region.x + 3) / (float)width;
	//	glyph->textureCoordinates.w = (region.y + 3) / (float)height;
	//	glyphs->Push((ui8*)&glyph);
	//	return glyph;
	//}

	return NULL;
}

struct OPfontBuildWordResult {
	ui32 count;
	f32 width;
};

OPfontBuildWordResult _OPfontBuildWord(OPfont* font, const char* text, OPfloat scale) {
	OPfontBuildWordResult result;
	result.count = 0;
	result.width = 0;
	for (ui32 i = 0; i < strlen(text); i++) {
		result.count++;
		if (text[i] == ' ') {
			break;
		}

		OPfontGlyph* glyph = font->GetGlyph(text[i]);
		if (glyph != NULL)
		{
			OPfloat kerning = 0;
			if (i > 0)
			{
				kerning = glyph->GetKerning(text[i - 1]);
			}
			result.width += kerning * scale;
			result.width += glyph->advanceX * scale;
		}
	}

	return result;
}

OPvec2 _OPfontBuildSep(OPvector* vertices, OPvector* indices, OPfont* font, const OPchar* text, OPfloat scale, f32 maxWidth) {
	OPfloat width = 0;
	OPfloat height = 0;
	size_t i = 0;
	OPfontBuildWordResult word = _OPfontBuildWord(font, text, scale);
	while (word.count > 0) {

		// Split it up by word
		if (maxWidth > 0 && width + word.width > maxWidth) {
			width = 0;
			height += font->height * scale;
		}

		for (ui32 j = 0; j < word.count; j++) {
			OPfontGlyph* glyph = font->GetGlyph(text[i]);
			if (glyph != NULL)
			{
				OPfloat kerning = 0;
				if (i > 0)
				{
					kerning = glyph->GetKerning(text[i - 1]);
				}
				width += kerning * scale;

				int x0 = (int)(width + glyph->offsetX * scale);
				int x1 = (int)(x0 + glyph->width * scale);
				f32 fontHeight = (font->height * scale - font->size * scale);
				int y0 = (int)(height + font->size * scale - (int)(glyph->offsetY * scale) - fontHeight);
				int y1 = (int)(height + font->size * scale - (int)(glyph->offsetY * scale - glyph->height * scale) - fontHeight);

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
			i++;
		}

		word = _OPfontBuildWord(font, &text[i], scale);
	}

	height += font->height * scale;

	return OPvec2(width, height);
}

OPvec2 _OPfontBuild(OPvector* vertices, OPvector* indices, OPfont* font, const OPchar* text, OPfloat scale, f32 maxWidth) {

	OPfloat width = 0;
	OPfloat height = 0;// font->height;

	size_t i;

	for (i = 0; i< strlen(text); ++i)
	{
		OPfontGlyph* glyph = font->GetGlyph(text[i]);
		if (glyph != NULL)
		{
			OPfloat kerning = 0;
			if (i > 0)
			{
				kerning = glyph->GetKerning( text[i - 1]);
			}
			width += kerning * scale;

			if (maxWidth > 0 && width > maxWidth && text[i] == ' ') {
				width = 0;
				height += font->height;
				continue;
			}

			int x0 = (int)(width + glyph->offsetX * scale);
			int x1 = (int)(x0 + glyph->width * scale);
			f32 fontHeight = (font->height * scale - font->size * scale);
			int y0 = (int)(height + font->size * scale - (int)(glyph->offsetY * scale) - fontHeight);
			int y1 = (int)(height + font->size * scale - (int)(glyph->offsetY * scale - glyph->height * scale) - fontHeight);

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
	height += font->height;

	return OPvec2(width, height);
}

OPmodel OPfont::CreateText(OPchar* text) {
	ui32 vertexSize = sizeof(OPvertexTex);
	OPindexSize::Enum indexSize = OPindexSize::SHORT;// sizeof(ui16);
	OPvector* vertices = OPvector::Create(vertexSize);
	OPvector* indices = OPvector::Create((ui32)indexSize);

	_OPfontBuild(vertices, indices, this, text, 1, 0);

	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::UV);
	OPvertexLayout vertexLayout = builder.Build();
	OPmodel mesh = OPmodel(1, vertexLayout);
	mesh.Build((ui32)vertices->_size, (ui32)indices->_size, indexSize, vertices->items, indices->items);
	return mesh;
}

OPfontBuiltTextNode OPfont::CreatePackedText(const OPchar* text) {
	return CreatePackedText(text, 1);
}

OPfontBuiltTextNode OPfont::CreatePackedText(const OPchar* text, OPfloat scale) {
	ASSERT(OPMESHPACKER_ACTIVE != NULL, "No mesh packer bound.");

	ui32 vertexSize = sizeof(OPvertexTex);
	OPindexSize::Enum indexSize = OPindexSize::SHORT;// sizeof(ui16);
	OPvector* vertices = OPvector::Create(vertexSize);
	OPvector* indices = OPvector::Create((ui32)indexSize);

	OPvec2 size = _OPfontBuild(vertices, indices, this, text, scale, 0);

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

OPvec2 OPfont::GetSize(const OPchar* text, OPfloat scale) {

	size_t i;
	OPfloat width = 0;
	OPfloat height = size * scale;
	for (i = 0; i< strlen(text); ++i)
	{
		OPfontGlyph* glyph = GetGlyph(text[i]);
		if (glyph != NULL)
		{
			if (i > 0)
				width += glyph->GetKerning(text[i - 1]) * scale;
			width += glyph->advanceX * scale;
		}
	}
	return OPvec2(width, height);
}

OPfontUserTextNode OPfont::CreateUserText(const OPchar* text) {
	return CreateUserText(text, 1);
}

OPfontUserTextNode OPfont::CreateUserText(const OPchar* text, float scale) {

	vertices->Clear();
	indices->Clear();

	OPvec2 size = _OPfontBuild(vertices, indices, this, text, scale, 0);

	dummyTextNode.Width = size.x;

	dummyTextNode.mesh.Build((ui32)vertices->_size, (ui32)indices->_size, OPindexSize::SHORT, vertices->items, indices->items);

	return dummyTextNode;
}


OPfontUserTextNode OPfont::CreateUserText(const OPchar* text, float scale, f32 maxWidth) {

	vertices->Clear();
	indices->Clear();

	OPvec2 size = _OPfontBuildSep(vertices, indices, this, text, scale, maxWidth);

	dummyTextNode.Width = size.x;

	dummyTextNode.mesh.Build((ui32)vertices->_size, (ui32)indices->_size, OPindexSize::SHORT, vertices->items, indices->items);

	return dummyTextNode;
}
