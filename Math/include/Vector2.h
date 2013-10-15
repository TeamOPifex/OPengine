#pragma once
#include "Core/include/Types.h"
#include "./Core/include/MathHelpers.h"

typedef struct {
	OPfloat x, y;
} OPvec2;

inline OPfloat* OPvec2index(OPvec2* v, int idx){
	return &((OPfloat*)(v))[idx];
}

inline void OPvec2add(OPvec2* dst, OPvec2* a, OPvec2* b){
	dst->x = a->x + b->x;
	dst->y = a->y + b->y;
}

inline void OPvec2sub(OPvec2* dst, OPvec2* a, OPvec2* b){
	dst->x = a->x - b->x;
	dst->y = a->y - b->y;
}

inline void OPvec2mul(OPvec2* dst, OPvec2* a, OPvec2* b){
	dst->x = a->x * b->x;
	dst->y = a->y * b->y;
}

inline void OPvec2scl(OPvec2* dst, OPfloat s){
	dst->x *= s;
	dst->y *= s;
}

inline void OPvec2div(OPvec2* dst, OPvec2* a, OPvec2* b){
	dst->x = a->x / b->x;
	dst->y = a->y / b->y;
}

inline void OPvec2dot(OPfloat* dst, OPvec2* a, OPvec2* b){
	(*dst) = a->x * b->x + a->y * b->y;
}

inline void OPvec2norm(OPvec2* dst, OPvec2* a){
	OPfloat l = sqrt(a->x * a->x + a->y * a->y);
	dst->x = a->x / l;
	dst->y = a->y / l;
}

inline void OPvec2perp(OPvec2* dst, OPvec2* a){
	dst->x = a->y;
	dst->y = a->x;
}