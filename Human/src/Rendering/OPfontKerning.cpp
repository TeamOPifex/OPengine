#pragma once

#include "./include/Rendering/OPfontKerning.h"
#include "./include/Rendering/OPfontHelpers.h"

void OPfontKerningGenerate(OPfont* font){
	OPint i, j;
	FT_Library library;
	FT_Face face;
	FT_UInt glyphIndex, prevIndex;
	OPfontGlyph* glyph, *prevGlyph;
	FT_Vector kerning;

	if (!OPfontGetFace(font, &library, &face)) {
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

    FT_Done_Face( face );
    FT_Done_FreeType( library );
}
