#ifndef OPENGINE_HUMAN_RENDERER_FONT_GLYPH
#define OPENGINE_HUMAN_RENDERER_FONT_GLYPH

#include "./Human/include/Rendering/Font/OPfontKerning.h"
#include "./Math/include/OPvec4.h"
#include "./Data/include/OPvector.h"

struct OPfontGlyph {
	OPchar charcode;
	OPint width;
	OPint height;
	OPint offsetX;
	OPint offsetY;
	OPfloat advanceX;
	OPfloat advanceY;
	OPvec4 textureCoordinates;
	OPvector* kerning;
	OPint outlineType;
	OPfloat outlineThickness;
};
typedef struct OPfontGlyph OPfontGlyph;

OPfontGlyph* OPfontGlyphCreate();
void OPfontGlyphDestroy(OPfontGlyph* glyph);
OPfloat OPfontGlyphGetKerning(OPfontGlyph* glyph, OPchar charcode);

#endif
