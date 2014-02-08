#pragma once

#include "./Human/include/Rendering/OPfontKerning.h"
#include "./Data/include/Vector.h"
#include "./Math/include/Vector4.h"

typedef struct {
	i8 charcode;
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

OPfontGlyph* OPfontGlyphCreate();
void OPfontGlyphDestroy(OPfontGlyph* glyph);
OPfloat OPfontGlyphGetKerning(OPfontGlyph* glyph, ui8 charcode);