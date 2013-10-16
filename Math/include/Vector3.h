#pragma once
#include "./Core/include/Types.h"
#include "./Core/include/MathHelpers.h"
#include "./Core/include/DynamicMemory.h"

struct OPvec3;
inline OPfloat* OPvec3index(OPvec3* v, int idx);
inline void OPvec3add(OPvec3* dst, OPvec3* a, OPvec3* b);
inline void OPvec3sub(OPvec3* dst, OPvec3* a, OPvec3* b);
inline void OPvec3mul(OPvec3* dst, OPvec3* a, OPvec3* b);
inline void OPvec3scl(OPvec3* dst, OPfloat s);
inline void OPvec3div(OPvec3* dst, OPvec3* a, OPvec3* b);
inline void OPvec3dot(OPfloat* dst, OPvec3* a, OPvec3* b);
inline void OPvec3cross(OPvec3* dst, OPvec3* a, OPvec3* b);
inline void OPvec3len(OPfloat* dst, OPvec3* v);
inline void OPvec3dist(OPfloat* dst, OPvec3* a, OPvec3* b);


struct OPvec3 {
	OPfloat x, y, z;
	OPvec3& operator=(OPvec3& vhs) { 
		OPmemcpy(this, &vhs, sizeof(OPvec3)); return *this;
	}
	inline OPvec3& operator+(OPvec3& vhs) { 
		OPvec3 temp;
		OPvec3add(&temp, this, &vhs); 
		return temp; 
	}
	inline OPvec3& operator+=(OPvec3& vhs) { 
		OPvec3add(this, this, &vhs); 
		return *this; 
	}
	inline OPvec3& operator-(OPvec3& vhs) { 
		OPvec3 temp;
		OPvec3sub(&temp, this, &vhs); 
		return temp; 
	}
	inline OPvec3& operator-=(OPvec3& vhs) { 
		OPvec3sub(this, this, &vhs); 
		return *this; 
	}
	inline OPvec3& operator*(OPvec3& vhs) { 
		OPvec3 temp;
		OPvec3mul(&temp, this, &vhs); 
		return temp; 
	}
	inline OPvec3& operator*=(OPvec3& vhs) { 
		OPvec3mul(this, this, &vhs); 
		return *this; 
	}
	inline OPvec3& operator/(OPvec3& vhs) { 
		OPvec3 temp;
		OPvec3div(&temp, this, &vhs); 
		return temp; 
	}
	inline OPvec3& operator/=(OPvec3& vhs) { 
		OPvec3div(this, this, &vhs); 
		return *this; 
	}
};

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

inline void OPvec3scl(OPvec3* dst, OPfloat s) {
	dst->x *= s;
	dst->y *= s;
	dst->z *= s;
}

inline void OPvec3div(OPvec3* dst, OPvec3* a, OPvec3* b) {
	dst->x = a->x / b->x;
	dst->y = a->y / b->y;
	dst->z = a->z / b->z;
}

inline void OPvec3dot(OPfloat* dst, OPvec3* a, OPvec3* b) {
	(*dst) = a->x * b->x + a->y * b->y + a->z * b->z;
}

inline void OPvec3norm(OPvec3* dst, OPvec3* a) {
	OPfloat l = sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
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
	(*dst) = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

inline void OPvec3dist(OPfloat* dst, OPvec3* a, OPvec3* b) {
	OPvec3 tmp;
	tmp.x = a->x - b->x;
	tmp.y = a->y - b->y;
	tmp.z = a->z - b->z;
	OPvec3len(dst, &tmp);
}