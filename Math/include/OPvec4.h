#pragma once
#ifndef OP_MATH_VECTOR4
#define OP_MATH_VECTOR4

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPmath.h"
#include "./Math/include/OPvec3.h"

struct OPvec4;
inline OPfloat* OPvec4Index(OPvec4* v, int idx);
inline void OPvec4Norm(OPvec4* dst, OPvec4* a);

inline OPvec4 OPvec4Create(OPfloat x, OPfloat y, OPfloat z, OPfloat w);
inline void OPvec4Add(OPvec4* dst, OPvec4* a, OPvec4* b);
inline void OPvec4Sub(OPvec4* dst, OPvec4* a, OPvec4* b);
inline void OPvec4Mul(OPvec4* dst, OPvec4* a, OPvec4* b);
inline void OPvec4Scl(OPvec4* dst, OPvec4* a, OPfloat s);
inline void OPvec4Div(OPvec4* dst, OPvec4* a, OPvec4* b);
inline void OPvec4Div(OPvec4* dst, OPvec4* a, OPfloat b);
inline void OPvec4Len(OPfloat* dst, OPvec4* v);
inline void OPvec4Dist(OPfloat* dst, OPvec4* a, OPvec4* b);

inline OPvec4 OPvec4Add(OPvec4* a, OPvec4* b);
inline OPvec4 OPvec4Sub(OPvec4* a, OPvec4* b);
inline OPvec4 OPvec4Mul(OPvec4* a, OPvec4* b);
inline OPvec4 OPvec4Scl(OPvec4* a, OPfloat s);
inline OPvec4 OPvec4Div(OPvec4* a, OPvec4* b);
inline OPvec4 OPvec4Div(OPvec4* a, OPfloat b);
inline OPfloat OPvec4Dot(OPvec4* a, OPvec4* b);
inline OPfloat OPvec4Len(OPvec4* v);
inline OPfloat OPvec4Dist(OPvec4* a, OPvec4* b);

struct OPvec4 {
	union{
		struct {
			OPfloat x, y, z, w;
		};
		OPfloat row[4];
	};
	OPvec4 operator=(OPvec4 vhs) { 
		OPmemcpy(this, &vhs, sizeof(OPvec4)); return *this;
	}
	inline OPvec4 operator+=(OPvec4 vhs) { 
		OPvec4Add(this, this, &vhs); 
		return *this; 
	}
	inline OPvec4 operator-=(OPvec4 vhs) { 
		OPvec4Sub(this, this, &vhs); 
		return *this; 
	}
	OPvec4 operator*=(OPmat4 rhs);
	inline OPvec4 operator*=(OPvec4 vhs) { 
		OPvec4Mul(this, this, &vhs); 
		return *this; 
	}
	inline OPvec4 operator*=(OPfloat vhs) { 
		OPvec4Scl(this, this, vhs); 
		return *this; 
	}
	inline OPvec4 operator/=(OPvec4 vhs) { 
		OPvec4Div(this, this, &vhs); 
		return *this; 
	}
	inline OPvec4 operator/=(OPfloat vhs) { 
		OPvec4Div(this, this, vhs); 
		return *this; 
	}
	inline OPfloat& operator[](i32 i) {
		return row[i];
	}
};

extern const OPvec4 OPvec4Zero;
extern const OPvec4 OPvec4One;


inline OPvec4 operator+(OPvec4 lhs, OPvec4 vhs) { 
	OPvec4 temp = { 0, 0, 0};
	OPvec4Add(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec4 operator-(OPvec4 lhs, OPvec4 vhs) { 
	OPvec4 temp = { 0, 0, 0};
	OPvec4Sub(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec4 operator*(OPvec4 lhs, OPvec4 vhs) { 
	OPvec4 temp = { 0, 0, 0};
	OPvec4Mul(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec4 operator*(OPvec4 lhs, OPfloat vhs) { 
	OPvec4 temp = { 0, 0, 0};
	OPvec4Scl(&temp, &lhs, vhs); 
	return temp; 
}
inline OPvec4 operator*(OPfloat lhs, OPvec4 vhs) { 
	OPvec4 temp = { 0, 0, 0};
	OPvec4Scl(&temp, &vhs, lhs); 
	return temp; 
}
inline OPvec4 operator/(OPvec4 lhs, OPvec4 vhs) { 
	OPvec4 temp = { 0, 0, 0};
	OPvec4Div(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec4 operator/(OPvec4 lhs, OPfloat vhs) { 
	OPvec4 temp = { 0, 0, 0};
	OPvec4Div(&temp, &lhs, vhs); 
	return temp; 
}
inline OPvec4 operator/(OPfloat lhs, OPvec4 vhs) { 
	OPvec4 temp = { 0, 0, 0};
	OPvec4Div(&temp, &vhs, lhs); 
	return temp; 
}


inline OPvec4 OPvec4Create(OPfloat x, OPfloat y, OPfloat z, OPfloat w) {
	OPvec4 tmp = { x, y, z, w };
	return tmp;
}

inline OPvec4 OPvec4CreateFromVec3(OPvec3 xyz, OPfloat w) {
	OPvec4 tmp = { xyz.x, xyz.y, xyz.z, w };
	return tmp;
}

inline OPfloat* OPvec4Index(OPvec4* v, int idx){
	return &((OPfloat*)(v))[idx];
}

inline void OPvec4Norm(OPvec4* dst, OPvec4* a) {
	OPfloat l = OPsqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
	dst->x = a->x / l;
	dst->y = a->x / l;
	dst->z = a->x / l;
	dst->w = a->x / l;
}


inline void OPvec4Add(OPvec4* dst, OPvec4* a, OPvec4* b) {
	dst->x = a->x + b->x;
	dst->y = a->y + b->y;
	dst->z = a->z + b->z;
	dst->w = a->w + b->w;
}

inline void OPvec4Sub(OPvec4* dst, OPvec4* a, OPvec4* b) {
	dst->x = a->x - b->x;
	dst->y = a->y - b->y;
	dst->z = a->z - b->z;
	dst->w = a->w - b->w;
}

inline void OPvec4Mul(OPvec4* dst, OPvec4* a, OPvec4* b) {
	dst->x = a->x * b->x;
	dst->y = a->y * b->y;
	dst->z = a->z * b->z;
	dst->w = a->w * b->w;
}

inline void OPvec4Scl(OPvec4* dst, OPvec4* a, OPfloat s) {
	dst->x = a->x * s;
	dst->y = a->y *s;
	dst->z = a->z *s;
	dst->w = a->w *s;
}

inline void OPvec4Div(OPvec4* dst, OPvec4* a, OPvec4* b) {
	dst->x = a->x / b->x;
	dst->y = a->y / b->y;
	dst->z = a->z / b->z;
	dst->z = a->w / b->w;
}

inline void OPvec4Div(OPvec4* dst, OPvec4* a, OPfloat b) {
	dst->x = a->x / b;
	dst->y = a->y / b;
	dst->z = a->z / b;
	dst->w = a->w / b;
}

inline void OPvec4Dot(OPfloat* dst, OPvec4* a, OPvec4* b) {
	(*dst) = a->x * b->x + a->y * b->y + a->z * b->z+ a->w * b->w;
}

inline void OPvec4Len(OPfloat* dst, OPvec4* v) {
	(*dst) = OPsqrt(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}

inline void OPvec4Dist(OPfloat* dst, OPvec4* a, OPvec4* b) {
	OPvec4 tmp;
	tmp.x = a->x - b->x;
	tmp.y = a->y - b->y;
	tmp.z = a->z - b->z;
	tmp.w = a->w - b->w;
	OPvec4Len(dst, &tmp);
}

inline OPvec4 OPvec4Add(OPvec4* a, OPvec4* b) {
	OPvec4 temp = { 0, 0, 0, 0};
	OPvec4Add(&temp, a, b);
	return temp;
}

inline OPvec4 OPvec4Sub(OPvec4* a, OPvec4* b) {
	OPvec4 temp = { 0, 0, 0, 0};
	OPvec4Sub(&temp, a, b);
	return temp;
}

inline OPvec4 OPvec4Mul(OPvec4* a, OPvec4* b) {
	OPvec4 temp = { 0, 0, 0, 0};
	OPvec4Mul(&temp, a, b);
	return temp;
}

inline OPvec4 OPvec4Scl(OPvec4* a, OPfloat s) {
	OPvec4 temp = { 0, 0, 0, 0};
	OPvec4Scl(&temp, a, s);
	return temp;
}

inline OPvec4 OPvec4Div(OPvec4* a, OPvec4* b) {
	OPvec4 temp = { 0, 0, 0, 0};
	OPvec4Div(&temp, a, b);
	return temp;
}

inline OPvec4 OPvec4Div(OPvec4* a, OPfloat b) {
	OPvec4 temp = { 0, 0, 0, 0};
	OPvec4Div(&temp, a, b);
	return temp;
}

inline OPfloat OPvec4Dot(OPvec4* a, OPvec4* b) {
	OPfloat temp = 0;
	OPvec4Dot(&temp, a, b);
	return temp;
}

inline OPfloat OPvec4Len(OPvec4* v) {
	OPfloat temp = 0;
	OPvec4Len(&temp, v);
	return temp;
}

inline OPfloat OPvec4Dist(OPvec4* a, OPvec4* b) {
	OPfloat temp = 0;
	OPvec4Dist(&temp, a, b);
	return temp;
}

inline OPvec4 OPvec4str(OPstream* str) {
	OPvec4 temp = {
		OPreadf32(str),
		OPreadf32(str),
		OPreadf32(str),
		OPreadf32(str)
	};
	return temp;
}

inline OPvec4 OPvec4clone(OPvec4* v) {
	return OPvec4Create(v->x, v->y, v->z, v->w);
}

inline void OPvec4write(OPvec4* v, OPstream* str) {
	OPwrite(str, &v->x, sizeof(f32));
	OPwrite(str, &v->y, sizeof(f32));
	OPwrite(str, &v->z, sizeof(f32));
	OPwrite(str, &v->w, sizeof(f32));
}

inline OPvec4 OPvec4randNorm(){
	OPvec4 v = {
		OPrandom() - 0.5f,
		OPrandom() - 0.5f,
		OPrandom() - 0.5f,
		OPrandom() - 0.5f
	};

	OPvec4Norm(&v, &v);

	return v;
}

inline void OPlogVec4(const OPchar* m, OPvec4* v) {
	OPlog("%s: [%f, %f, %f, %f]", m, v->x, v->y, v->z, v->w);
}
#endif