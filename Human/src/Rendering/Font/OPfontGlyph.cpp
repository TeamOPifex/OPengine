#include "./Human/include/Rendering/Font/OPfontGlyph.h"

void OPfontGlyph::Init() {
	OPbzero(this, sizeof(OPfontGlyph));
	kerning = OPvector::Create(sizeof(OPfontKerning));
}

void OPfontGlyph::Destroy() {
	kerning->Destroy();
	OPfree(kerning);
}

OPfloat OPfontGlyph::GetKerning(OPchar charcode){
	OPint i;
	for (i = 0; i < kerning->Size(); ++i){
		OPfontKerning* fontKerning = (OPfontKerning*)kerning->Get(i);
		if (fontKerning->charcode == charcode) {
			return fontKerning->kerning;
		}
	}
	return 0;
}
