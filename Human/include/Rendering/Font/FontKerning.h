#ifndef OPENGINE_HUMAN_RENDERER_FONT_KERNING
#define OPENGINE_HUMAN_RENDERER_FONT_KERNING

#include "./Core/include/Types.h"

typedef struct {
	OPchar charcode;
	OPfloat kerning;
} OPfontKerning;

#endif