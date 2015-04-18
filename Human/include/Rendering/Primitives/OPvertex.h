#ifndef OPENGINE_HUMAN_RENDERER_PRIMITIVES_OPVERTEX
#define OPENGINE_HUMAN_RENDERER_PRIMITIVES_OPVERTEX

#include "./Math/include/Vectors.h"

typedef struct {
	OPvec3 Position;
	OPvec3 Color;
} OPvertexColor;

typedef struct {
	OPvec3 Position;
	OPvec2 TexCoord;
	OPvec3 Color;
} OPvertexTexColor;

#endif