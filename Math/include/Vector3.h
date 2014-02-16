#ifndef OP_MATH_VECTOR3
#define OP_MATH_VECTOR3
#include "./Core/include/Types.h"
#include "./Core/include/MathHelpers.h"
#include "./Core/include/DynamicMemory.h"

struct OPvec3;
struct OPmat4;

inline OPfloat* OPvec3index(OPvec3* v, int idx);
inline void OPvec3add(OPvec3* dst, OPvec3* a, OPvec3* b);
inline void OPvec3sub(OPvec3* dst, OPvec3* a, OPvec3* b);
inline void OPvec3mul(OPvec3* dst, OPvec3* a, OPvec3* b);
inline void OPvec3scl(OPvec3* dst, OPvec3* a, OPfloat s);
inline void OPvec3div(OPvec3* dst, OPvec3* a, OPvec3* b);
inline void OPvec3divf(OPvec3* dst, OPvec3* a, OPfloat b);
inline void OPvec3dot(OPfloat* dst, OPvec3* a, OPvec3* b);
inline void OPvec3cross(OPvec3* dst, OPvec3* a, OPvec3* b);
inline void OPvec3len(OPfloat* dst, OPvec3* v);
inline void OPvec3dist(OPfloat* dst, OPvec3* a, OPvec3* b);

inline OPvec3 OPvec3valAdd(OPvec3* a, OPvec3* b);
inline OPvec3 OPvec3valSub(OPvec3* a, OPvec3* b);
inline OPvec3 OPvec3valMul(OPvec3* a, OPvec3* b);
inline OPvec3 OPvec3valScl(OPvec3* a, OPfloat s);
inline OPvec3 OPvec3valDiv(OPvec3* a, OPvec3* b);
inline OPvec3 OPvec3valDivf(OPvec3* a, OPfloat b);
inline OPfloat OPvec3valDot(OPvec3* a, OPvec3* b);
inline OPvec3 OPvec3valCross(OPvec3* a, OPvec3* b);
inline OPfloat OPvec3valLen(OPvec3* v);
inline OPfloat OPvec3valDist(OPvec3* a, OPvec3* b);


struct OPvec3 {
	OPfloat x, y, z;
	inline OPvec3 operator=(OPvec3 vhs) { 
		OPmemcpy(this, (void*)&vhs, sizeof(OPvec3)); return *this;
	}
	inline OPvec3 operator+=(OPvec3 vhs) { 
		OPvec3add(this, this, &vhs); 
		return *this; 
	}
	inline OPvec3 operator-=(OPvec3 vhs) { 
		OPvec3sub(this, this, &vhs); 
		return *this; 
	}
	OPvec3 operator*=(OPmat4 rhs);
	inline OPvec3 operator*=(OPvec3 vhs) { 
		OPvec3mul(this, this, &vhs); 
		return *this; 
	}
	inline OPvec3 operator*=(OPfloat vhs) { 
		OPvec3scl(this, this, vhs); 
		return *this; 
	}
	inline OPvec3 operator/=(OPvec3 vhs) { 
		OPvec3div(this, this, &vhs); 
		return *this; 
	}
	inline OPvec3 operator/=(OPfloat vhs) { 
		OPvec3divf(this, this, vhs); 
		return *this; 
	}
};

extern const OPvec3 OPvec3Zero;
extern const OPvec3 OPvec3One;

inline OPvec3 operator+(OPvec3 lhs, OPvec3& vhs) { 
	OPvec3 temp = { 0, 0, 0};
	OPvec3add(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec3 operator-(OPvec3 lhs, OPvec3& vhs) { 
	OPvec3 temp = { 0, 0, 0};
	OPvec3sub(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec3 operator*(OPvec3 lhs, OPvec3& vhs) { 
	OPvec3 temp = { 0, 0, 0};
	OPvec3mul(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec3 operator*(OPvec3 lhs, OPfloat vhs) { 
	OPvec3 temp = { 0, 0, 0};
	OPvec3scl(&temp, &lhs, vhs); 
	return temp; 
}
inline OPvec3 operator*(OPfloat lhs, OPvec3 vhs) { 
	OPvec3 temp = { 0, 0, 0};
	OPvec3scl(&temp, &vhs, lhs); 
	return temp; 
}
inline OPvec3 operator/(OPvec3 lhs, OPvec3& vhs) { 
	OPvec3 temp = { 0, 0, 0};
	OPvec3div(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec3 operator/(OPvec3 lhs, OPfloat vhs) { 
	OPvec3 temp = { 0, 0, 0};
	OPvec3divf(&temp, &lhs, vhs); 
	return temp; 
}
inline OPvec3 operator/(OPfloat lhs, OPvec3& vhs) { 
	OPvec3 temp = { 0, 0, 0};
	OPvec3divf(&temp, &vhs, lhs); 
	return temp; 
}


inline OPfloat* OPvec3index(OPvec3* v, int idx) {
	return &((OPfloat*)(v))[idx];
}

inline void OPvec3add(OPvec3* dst, OPvec3* a, OPvec3* b) {
	dst->x = a->x + b->x;
	dst->y = a->y + b->y;
	dst->z = a->z + b->z;
}

inline void OPvec3sub(OPvec3* dst, OPvec3* a, OPvec3* b) {
	dst->x = a->x - b->x;
	dst->y = a->y - b->y;
	dst->z = a->z - b->z;
}

inline void OPvec3mul(OPvec3* dst, OPvec3* a, OPvec3* b) {
	dst->x = a->x * b->x;
	dst->y = a->y * b->y;
	dst->z = a->z * b->z;
}

inline void OPvec3scl(OPvec3* dst, OPvec3* a, OPfloat s) {
	dst->x = a->x * s;
	dst->y = a->y *s;
	dst->z = a->z *s;
}

inline void OPvec3div(OPvec3* dst, OPvec3* a, OPvec3* b) {
	dst->x = a->x / b->x;
	dst->y = a->y / b->y;
	dst->z = a->z / b->z;
}

inline void OPvec3divf(OPvec3* dst, OPvec3* a, OPfloat b) {
	dst->x = a->x / b;
	dst->y = a->y / b;
	dst->z = a->z / b;
}

inline void OPvec3dot(OPfloat* dst, OPvec3* a, OPvec3* b) {
	(*dst) = a->x * b->x + a->y * b->y + a->z * b->z;
}

inline void OPvec3norm(OPvec3* dst, OPvec3* a) {
	OPfloat l = OPsqrt(a->x * a->x + a->y * a->y + a->z * a->z);
	if(l == 0) {
		return;
	}
	dst->x = a->x / l;
	dst->y = a->y / l;
	dst->z = a->z / l;
}

inline void OPvec3cross(OPvec3* dst, OPvec3* a, OPvec3* b) {
	OPfloat x = a->y * b->z - a->z * b->y;
	OPfloat y = a->z * b->x - a->x * b->z;
	OPfloat z = a->x * b->y - a->y * b->x;
	dst->x = x;
	dst->y = y;
	dst->z = z;
}

inline void OPvec3len(OPfloat* dst, OPvec3* v) {
	(*dst) = OPsqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

inline void OPvec3dist(OPfloat* dst, OPvec3* a, OPvec3* b) {
	OPvec3 tmp;
	tmp.x = a->x - b->x;
	tmp.y = a->y - b->y;
	tmp.z = a->z - b->z;
	OPvec3len(dst, &tmp);
}

inline OPvec3 OPvec3valAdd(OPvec3* a, OPvec3* b) {
	OPvec3 temp = { 0, 0, 0};
	OPvec3add(&temp, a, b);
	return temp;
}

inline OPvec3 OPvec3valSub(OPvec3* a, OPvec3* b) {
	OPvec3 temp = { 0, 0, 0};
	OPvec3sub(&temp, a, b);
	return temp;
}

inline OPvec3 OPvec3valMul(OPvec3* a, OPvec3* b) {
	OPvec3 temp = { 0, 0, 0};
	OPvec3mul(&temp, a, b);
	return temp;
}

inline OPvec3 OPvec3valScl(OPvec3* a, OPfloat s) {
	OPvec3 temp = { 0, 0, 0};
	OPvec3scl(&temp, a, s);
	return temp;
}

inline OPvec3 OPvec3valDiv(OPvec3* a, OPvec3* b) {
	OPvec3 temp = { 0, 0, 0};
	OPvec3div(&temp, a, b);
	return temp;
}

inline OPvec3 OPvec3valDivf(OPvec3* a, OPfloat b) {
	OPvec3 temp = { 0, 0, 0};
	OPvec3divf(&temp, a, b);
	return temp;
}

inline OPfloat OPvec3valDot(OPvec3* a, OPvec3* b) {
	OPfloat temp = 0;
	OPvec3dot(&temp, a, b);
	return temp;
}

inline OPvec3 OPvec3valCross(OPvec3* a, OPvec3* b) {
	OPvec3 temp = { 0, 0, 0};
	OPvec3cross(&temp, a, b);
	return temp;
}

inline OPfloat OPvec3valLen(OPvec3* v) {
	OPfloat temp = 0;
	OPvec3len(&temp, v);
	return temp;
}

inline OPfloat OPvec3valDist(OPvec3* a, OPvec3* b) {
	OPfloat temp = 0;
	OPvec3dist(&temp, a, b);
	return temp;
}
#endif