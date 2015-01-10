#ifndef OPENGINE_HUMAN_RENDERER_FONT_GLYPH
#define OPENGINE_HUMAN_RENDERER_FONT_GLYPH

#include "./Human/include/Rendering/Font/OPfontKerning.h"
#include "./Math/include/OPvec4.h"
#include "./Data/include/OPvector.h"

typedef struct {
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
} OPfontGlyph;

EXPORT_METHOD OPfontGlyph* OPfontGlyphCreate();
EXPORT_METHOD void OPfontGlyphDestroy(OPfontGlyph* glyph);
EXPORT_METHOD OPfloat OPfontGlyphGetKerning(OPfontGlyph* glyph, OPchar charcode);

#endif