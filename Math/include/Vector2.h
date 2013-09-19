#pragma once
#include "Core/include/Types.h"
#include "./Core/include/MathHelpers.h"

typedef struct {
	OPfloat x, y;
} OPvec2;

inline void vec2add(OPvec2* dst, OPvec2* a, OPvec2* b){
	dst->x = a->x + b->x;
	dst->y = a->y + b->y;
}

inline void vec2sub(OPvec2* dst, OPvec2* a, OPvec2* b){
	dst->x = a->x - b->x;
	dst->y = a->y - b->y;
}

inline void vec2mul(OPvec2* dst, OPvec2* a, OPvec2* b){
	dst->x = a->x * b->x;
	dst->y = a->y * b->y;
}

inline void vec2scl(OPvec2* dst, OPfloat s){
	dst->x *= s;
	dst->y *= s;
}

inline void vec2div(OPvec2* dst, OPvec2* a, OPvec2* b){
	dst->x = a->x / b->x;
	dst->y = a->y / b->y;
}

inline void vec2dot(OPfloat* dst, OPvec2* a, OPvec2* b){
	(*dst) = a->x * b->x + a->y * b->y;
}

inline void vec2norm(OPvec2* dst, OPvec2* a){
	OPfloat l = sqrt(a->x * a->x + a->y * a->y);
	dst->x = a->x / l;
	dst->y = a->y / l;
}

inline void vec2perp(OPvec2* dst, OPvec2* a){
	dst->x = a->y;
	dst->y = a->x;
}