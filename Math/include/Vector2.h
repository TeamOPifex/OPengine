#pragma once
#include "Core/include/Types.h"
#include "./Core/include/MathHelpers.h"
#include "./Core/include/DynamicMemory.h"

struct OPvec2;
inline OPfloat* OPvec2index(OPvec2* v, int idx);
inline void OPvec2add(OPvec2* dst, OPvec2* a, OPvec2* b);
inline void OPvec2sub(OPvec2* dst, OPvec2* a, OPvec2* b);
inline void OPvec2mul(OPvec2* dst, OPvec2* a, OPvec2* b);
inline void OPvec2scl(OPvec2* dst, OPvec2* a, OPfloat s);
inline void OPvec2div(OPvec2* dst, OPvec2* a, OPvec2* b);
inline void OPvec2divf(OPvec2* dst, OPvec2* a, OPfloat b);
inline void OPvec2dot(OPfloat* dst, OPvec2* a, OPvec2* b);
inline void OPvec2len(OPfloat* dst, OPvec2* v);
inline void OPvec2norm(OPvec2* dst, OPvec2* a);
inline void OPvec2perp(OPvec2* dst, OPvec2* a);

inline OPvec2 OPvec2valAdd(OPvec2* a, OPvec2* b);
inline OPvec2 OPvec2valSub(OPvec2* a, OPvec2* b);
inline OPvec2 OPvec2valMul(OPvec2* a, OPvec2* b);
inline OPvec2 OPvec2valScl(OPvec2* a, OPfloat s);
inline OPvec2 OPvec2valDiv(OPvec2* a, OPvec2* b);
inline OPvec2 OPvec2valDivf(OPvec2* a, OPfloat b);
inline OPfloat OPvec2valDot(OPvec2* a, OPvec2* b);
inline OPfloat OPvec2valLen(OPvec2* v);
inline OPvec2 OPvec2valNorm(OPvec2* a);
inline OPvec2 OPvec2valPerp(OPvec2* a);

struct OPvec2 {
	OPfloat x, y;

	OPvec2 operator=(OPvec2 vhs) { 
		OPmemcpy(this, &vhs, sizeof(OPvec2)); return *this;
	}
	inline OPvec2 operator+=(OPvec2 vhs) { 
		OPvec2add(this, this, &vhs); 
		return *this; 
	}
	inline OPvec2 operator-=(OPvec2 vhs) { 
		OPvec2sub(this, this, &vhs); 
		return *this; 
	}
	inline OPvec2 operator*=(OPvec2 vhs) { 
		OPvec2mul(this, this, &vhs); 
		return *this; 
	}
	inline OPvec2 operator*=(OPfloat vhs) { 
		OPvec2scl(this, this, vhs); 
		return *this; 
	}
	inline OPvec2 operator/=(OPvec2 vhs) { 
		OPvec2div(this, this, &vhs); 
		return *this; 
	}
	inline OPvec2 operator/=(OPfloat vhs) { 
		OPvec2divf(this, this, vhs); 
		return *this; 
	}
};

inline OPvec2 operator+(OPvec2 lhs, OPvec2 vhs) { 
	OPvec2 temp = { 0.0, 0.0 };
	OPvec2add(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec2 operator-(OPvec2 lhs, OPvec2 vhs) { 
	OPvec2 temp = { 0.0, 0.0 };
	OPvec2sub(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec2 operator*(OPvec2 lhs, OPvec2 vhs) { 
	OPvec2 temp = { 0.0, 0.0 };
	OPvec2mul(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec2 operator*(OPvec2 lhs, OPfloat vhs) { 
	OPvec2 temp = { 0.0, 0.0 };
	OPvec2scl(&temp, &lhs, vhs); 
	return temp; 
}
inline OPvec2 operator*(OPfloat lhs, OPvec2 vhs) { 
	OPvec2 temp = { 0.0, 0.0 };
	OPvec2scl(&temp, &vhs, lhs); 
	return temp; 
}
inline OPvec2 operator/(OPvec2 lhs, OPvec2 vhs) { 
	OPvec2 temp = { 0.0, 0.0 };
	OPvec2div(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec2 operator/(OPvec2 lhs, OPfloat vhs) { 
	OPvec2 temp = { 0.0, 0.0 };
	OPvec2divf(&temp, &lhs, vhs); 
	return temp; 
}

inline OPvec2 operator/(OPfloat lhs, OPvec2 vhs) { 
	OPvec2 temp = { 0.0, 0.0 };
	OPvec2divf(&temp, &vhs, lhs); 
	return temp; 
}


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

inline void OPvec2scl(OPvec2* dst, OPvec2* a, OPfloat s){
	dst->x = a->x * s;
	dst->y = a->y * s;
}

inline void OPvec2div(OPvec2* dst, OPvec2* a, OPvec2* b){
	dst->x = a->x / b->x;
	dst->y = a->y / b->y;
}
inline void OPvec2divf(OPvec2* dst, OPvec2* a, OPfloat b){
	dst->x = a->x / b;
	dst->y = a->y / b;
}

inline void OPvec2dot(OPfloat* dst, OPvec2* a, OPvec2* b){
	(*dst) = a->x * b->x + a->y * b->y;
}

inline void OPvec2len(OPfloat* dst, OPvec2* v){
	(*dst) = sqrt(v->x * v->x + v->y * v->y);
}

inline void OPvec2norm(OPvec2* dst, OPvec2* a){
	OPfloat l = sqrt(a->x * a->x + a->y * a->y);
	if(l == 0) {
		return;	
	}
	dst->x = a->x / l;
	dst->y = a->y / l;
}

inline void OPvec2perp(OPvec2* dst, OPvec2* a){
	dst->x = a->y;
	dst->y = a->x;
}


inline OPvec2 OPvec2valAdd(OPvec2* a, OPvec2* b) { 
	OPvec2 temp;
	OPvec2add(&temp, a, b);
	return temp;
}

inline OPvec2 OPvec2valSub(OPvec2* a, OPvec2* b) { 
	OPvec2 temp;
	OPvec2add(&temp, a, b);
	return temp;
}

inline OPvec2 OPvec2valMul(OPvec2* a, OPvec2* b) { 
	OPvec2 temp;
	OPvec2mul(&temp, a, b);
	return temp;
}

inline OPvec2 OPvec2valScl(OPvec2* a, OPfloat s) { 
	OPvec2 temp;
	OPvec2scl(&temp, a, s);
	return temp;
}

inline OPvec2 OPvec2valDiv(OPvec2* a, OPvec2* b) { 
	OPvec2 temp;
	OPvec2div(&temp, a, b);
	return temp;
}

inline OPvec2 OPvec2valDivf(OPvec2* a, OPfloat b) { 
	OPvec2 temp;
	OPvec2divf(&temp, a, b);
	return temp;
}

inline OPfloat OPvec2valDot(OPvec2* a, OPvec2* b) { 
	OPfloat temp;
	OPvec2dot(&temp, a, b);
	return temp;
}

inline OPfloat OPvec2valLen(OPvec2* v) { 
	OPfloat temp;
	OPvec2len(&temp, v);
	return temp;
}

inline OPvec2 OPvec2valNorm(OPvec2* a) { 
	OPvec2 temp;
	OPvec2norm(&temp, a);
	return temp;
}

inline OPvec2 OPvec2valPerp(OPvec2* a) { 
	OPvec2 temp;
	OPvec2perp(&temp, a);
	return temp;
}

inline OPvec2 OPvec2valReflect(OPvec2* horizon, OPvec2* v){
	OPfloat ratio = 2 * (OPvec2valDot(v, horizon) / OPvec2valDot(horizon, horizon));
	return *horizon * ratio - *v;
}
