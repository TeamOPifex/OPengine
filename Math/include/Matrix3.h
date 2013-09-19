#pragma once

#include "Vector3.h"
#include "./Core/include/DynamicMemory.h"

typedef struct {
	OPvec3 cols[3];
} OPmat3;

inline OPvec3* OPmat3index(OPmat3* m, int idx){
	return &((OPvec3*)(m))[idx];
}

inline void OPmat3identity(OPmat3* dst) {
	OPbzero(dst, sizeof(OPmat3));
	dst->cols[0].x = 1;
	dst->cols[1].y = 1;
	dst->cols[2].z = 1;
}