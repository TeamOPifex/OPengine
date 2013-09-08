#pragma once
#include "./Core/include/Types.h"
#include "./Core/include/MathHelpers.h"

#define OPvec3add(dst, a, b){\
	dst.x = a.x + b.x;\
	dst.y = a.y + b.y;\
	dst.z = a.z + b.z;\
}\

#define OPvec3sub(dst, a, b){\
	dst.x = a.x - b.x;\
	dst.y = a.y - b.y;\
	dst.z = a.z - b.z;\
}\

#define OPvec3mul(dst, a, b){\
	dst.x = a.x * b.x;\
	dst.y = a.y * b.y;\
	dst.z = a.z * b.z;\
}\

#define OPvec3scl(dst, s){\
	dst.x *= s;\
	dst.y *= s;\
	dst.z *= s;\
}\

#define OPvec3div(dst, a, b){\
	dst.x = a.x / b.x;\
	dst.y = a.y / b.y;\
	dst.z = a.z / b.z;\
}\

#define OPvec3dot(dst, a, b){\
	dst = a.x * b.x + a.y * b.y + a.z * b.z;\
}\

#define OPvec3norm(dst, a){\
	OPfloat l = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);\
	dst.x = a.x / l;\
	dst.y = a.y / l;\
	dst.z = a.z / l;\
}\

#define OPvec3cross(dst, a, b){\
	OPfloat x = a.y * b.z - a.z * b.y;\
	OPfloat y = a.z * b.x - a.x * b.z;\
	OPfloat z = a.x * b.y - a.y * b.x;\
	dst.x = x;\
	dst.y = y;\
	dst.z = z;\
}\

#define OPvec3len(dst, v) {\
	dst = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);\
}\

#define OPvec3dist(dst, a, b) {\
	OPvec3 tmp;\
	tmp.x = a.x - b.x;\
	tmp.y = a.y - b.y;\
	tmp.z = a.z - b.z;\
	vec3len(dst, tmp);\
}\

typedef struct {
	OPfloat x, y, z;
} OPvec3;