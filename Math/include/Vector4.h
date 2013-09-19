#pragma once

#include "./Core/include/Types.h"

typedef struct {
	OPfloat x, y, z, w;
} OPvec4;

inline OPfloat* OPvec4index(OPvec4* v, int idx){
	return &((OPfloat*)(v))[idx];
}

inline void OPvec4norm(OPvec4* dst, OPvec4* a) {
	OPfloat l = OPsqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
	dst->x = a->x / l;
	dst->y = a->x / l;
	dst->z = a->x / l;
	dst->w = a->x / l;
}