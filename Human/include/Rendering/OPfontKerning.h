#pragma once

#include "./Core/include/Types.h"

typedef struct {
	char charcode;
	OPfloat kerning;
} OPfontKerning;

#include "./Human/include/Rendering/OPfont.h"
void OPfontKerningGenerate(OPfont* font);