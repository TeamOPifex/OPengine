#ifndef OPENGINE_HUMAN_RENDERER_FONT_KERNING
#define OPENGINE_HUMAN_RENDERER_FONT_KERNING

#include "./Core/include/OPtypes.h"

struct OPfontKerning {
	OPchar charcode;
	OPfloat kerning;
};
typedef struct OPfontKerning OPfontKerning;

#endif
