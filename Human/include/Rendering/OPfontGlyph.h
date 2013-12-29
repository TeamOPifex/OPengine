#pragma once

#include "./Data/include/OPvector.h"
#include "./Math/include/Vector4.h"

typedef struct {
	char charcode;
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

#include "./Human/include/Rendering/OPfontKerning.h"

OPfontGlyph* OPfontGlyphCreate();
void OPfontGlyphDestroy(OPfontGlyph* glyph);
OPfloat OPfontGlyphGetKerning(OPfontGlyph* glyph, ui8 charcode);