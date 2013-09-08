#pragma once

#include "Vector3.h"

#define OPmat3identity(dst) {\
	OPbzero(dst, sizeof(OPmat3));\
	dst[0][0] = 1;\
	dst[1][1] = 1;\
	dst[2][2] = 1;\
}\

typedef struct {
	OPvec3 cols[3];
} OPmat3;