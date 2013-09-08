#pragma once

#include "./Core/include/Types.h"

#define OPvec4norm(dst, a) {\
	OPfloat l = sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w); \
	dst.x = a.x / l;\
	dst.y = a.x / l;\
	dst.z = a.x / l;\
	dst.w = a.x / l;\
}\

typedef struct {
	OPfloat x, y, z, w;
} OPvec4;