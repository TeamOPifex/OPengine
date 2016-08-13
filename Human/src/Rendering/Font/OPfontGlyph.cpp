#include "./Human/include/Rendering/Font/OPfontGlyph.h"

OPfontGlyph* OPfontGlyphCreate() {
	OPfontGlyph* self = (OPfontGlyph*)OPalloc(sizeof(OPfontGlyph));
	OPbzero(self, sizeof(OPfontGlyph));
	self->kerning = OPvector::Create(sizeof(OPfontKerning));
	return self;
}

void OPfontGlyphDestroy(OPfontGlyph* glyph) {
	glyph->kerning->Destroy();
	OPfree(glyph->kerning);
	OPfree(glyph);
}

OPfloat OPfontGlyphGetKerning(OPfontGlyph* glyph, OPchar charcode){
	OPint i;
	for (i = 0; i < glyph->kerning->Size(); ++i){
		OPfontKerning* kerning = (OPfontKerning*)glyph->kerning->Get(i);
		if (kerning->charcode == charcode) {
			return kerning->kerning;
		}
	}
	return 0;
}
