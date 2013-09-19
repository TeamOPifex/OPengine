#pragma once

#include "Vector3.h"
#include "./Core/include/DynamicMemory.h"

typedef struct {
	OPvec3 cols[3];
} OPmat3;

inline void OPmat3identity(OPmat3* dst) {
	OPbzero(dst, sizeof(OPmat3));
	dst->cols[0].x = 1;
	dst->cols[1].y = 1;
	dst->cols[2].z = 1;
}