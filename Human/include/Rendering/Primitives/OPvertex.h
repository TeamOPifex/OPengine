#pragma once

struct OPvertexColor;
struct OPvertexTexColor;

typedef struct OPvertexColor OPvertexColor;
typedef struct OPvertexTexColor OPvertexTexColor;

#include "./Math/include/Vectors.h"

struct OPvertexColor {
	OPvec3 Position;
	OPvec3 Color;
};

struct OPvertexTexColor {
	OPvec3 Position;
	OPvec2 TexCoord;
	OPvec3 Color;
};