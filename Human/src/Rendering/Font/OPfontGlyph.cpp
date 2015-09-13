#include "./Human/include/Rendering/Font/OPfontGlyph.h"

OPfontGlyph* OPfontGlyphCreate() {
	OPfontGlyph* self = (OPfontGlyph*)OPalloc(sizeof(OPfontGlyph));
	OPbzero(self, sizeof(OPfontGlyph));
	self->kerning = OPvectorCreate(sizeof(OPfontKerning));
	return self;
}

void OPfontGlyphDestroy(OPfontGlyph* glyph) {
	OPvectorDestroy(glyph->kerning);
	OPfree(glyph->kerning);
	OPfree(glyph);
}

OPfloat OPfontGlyphGetKerning(OPfontGlyph* glyph, OPchar charcode){
	OPint i;
	for (i = 0; i < OPvectorSize(glyph->kerning); ++i){
		OPfontKerning* kerning = (OPfontKerning*)OPvectorGet(glyph->kerning, i);
		if (kerning->charcode == charcode) {
			return kerning->kerning;
		}
	}
	return 0;
}
