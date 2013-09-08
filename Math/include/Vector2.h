#pragma once
#include "Core/include/Types.h"

#define vec2add(dst, a, b){\
	dst.x = a.x + b.x;\
	dst.y = a.y + b.y;\
}\

#define vec2sub(dst, a, b){\
	dst.x = a.x - b.x;\
	dst.y = a.y - b.y;\
}\

#define vec2mul(dst, a, b){\
	dst.x = a.x * b.x;\
	dst.y = a.y * b.y;\
}\

#define vec2scl(dst, s){\
	dst.x *= s;\
	dst.y *= s;\
}\

#define vec2div(dst, a, b){\
	dst.x = a.x / b.x;\
	dst.y = a.y / b.y;\
}\

#define vec2dot(dst, a, b){\
	dst = a.x * b.x + a.y * b.y;\
}\

#define vec2norm(dst, a){\
	OPfloat l = sqrt(a.x * a.x + a.y * a.y);\
	dst.x = a.x / l;\
	dst.y = a.y / l;\
}\

#define vec2perp(dst, a){\
	dst.x = a.y;\
	dst.y = a.x;\
}\

typedef struct {
	OPfloat x, y;
} OPvec2;