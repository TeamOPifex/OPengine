#include "./Pipeline/include/Loaders/OPloaderOPF.h"
#include "./Pipeline/include/Loaders/OPloaderPNG.h"
#include "./Human/include/Utilities/OPimagePNG.h"

OPint OPfontLoad(OPstream* str, OPfont** data) {
	OPfont* font = OPNEW(OPfont());

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
		OPfontGlyph* glyph = OPNEW(OPfontGlyph());
		glyph->Init();

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

	font->Init();

	return 1;
}


OPint OPfontReload(OPstream* str, OPfont** font) {
	OPlog("Reload Image Font");
	//OPstream* str = OPreadFile(filename);
	OPfont* resultFont;
	OPfont* tex = (OPfont*)(*font);
	i32 result = OPfontLoad(str, &resultFont);
	if (result) {
		OPmemcpy(*font, resultFont, sizeof(OPfont));
		OPfree(resultFont);
	}
	return result;
}

OPint OPfontUnload(OPfont* font)
{
	font->Destroy();
	OPimagePNGUnload(font->texture);
	OPfree(font);
	return 1;
}

OPassetLoader OPASSETLOADER_OPF = {
	".opf",
	"Fonts/",
	sizeof(OPfont),
	(OPint(*)(OPstream*, void**))OPfontLoad,
	(OPint(*)(void*))OPfontUnload,
	(OPint(*)(OPstream*, void**))OPfontReload,
};