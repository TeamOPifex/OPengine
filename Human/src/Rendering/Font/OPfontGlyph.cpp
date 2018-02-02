#include "./Human/include/Rendering/Font/OPfontGlyph.h"

void OPfontGlyph::Init() {
	//OPbzero(this, sizeof(OPfontGlyph));
	//kerning = OPvector::Create(sizeof(OPfontKerning));
}

void OPfontGlyph::Destroy() {
	OPfree(kerning);
	//kerning->Destroy();
	//OPfree(kerning);
}

OPfloat OPfontGlyph::GetKerning(OPchar charcode){
	for (ui32 i = 0; i < kerningCount; ++i){
		if (kerning[i].charcode == charcode) {
			return kerning[i].kerning;
		}
	}
	return 0;
}
