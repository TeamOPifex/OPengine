#ifndef OPENGINE_HUMAN_RENDERER_PRIMITIVES_OPVERTEX
#define OPENGINE_HUMAN_RENDERER_PRIMITIVES_OPVERTEX

#include "./Math/include/Vectors.h"

struct OPvertexColor {
	OPvec3 Position;
	OPvec3 Color;
};
typedef struct OPvertexColor OPvertexColor;

struct OPvertexTexColor {
	OPvec3 Position;
	OPvec2 TexCoord;
	OPvec3 Color;
};
typedef struct OPvertexTexColor OPvertexTexColor;

#endif
