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
inline void OPvec2dot(OPfloat* dst, OPvec2* a, OPvec2* b);
inline void OPvec2norm(OPvec2* dst, OPvec2* a);
inline void OPvec2perp(OPvec2* dst, OPvec2* a);

struct OPvec2 {
	OPfloat x, y;

	OPvec2& operator=(OPvec2& vhs) { 
		OPmemcpy(this, &vhs, sizeof(OPvec2)); return *this;
	}
	inline OPvec2& operator+(OPvec2& vhs) { 
		OPvec2 temp;
		OPvec2add(&temp, this, &vhs); 
		return temp; 
	}
	inline OPvec2& operator+=(OPvec2& vhs) { 
		OPvec2add(this, this, &vhs); 
		return *this; 
	}
	inline OPvec2& operator-(OPvec2& vhs) { 
		OPvec2 temp;
		OPvec2sub(&temp, this, &vhs); 
		return temp; 
	}
	inline OPvec2& operator-=(OPvec2& vhs) { 
		OPvec2sub(this, this, &vhs); 
		return *this; 
	}
	inline OPvec2& operator*(OPvec2& vhs) { 
		OPvec2 temp;
		OPvec2mul(&temp, this, &vhs); 
		return temp; 
	}
	inline OPvec2& operator*=(OPvec2& vhs) { 
		OPvec2mul(this, this, &vhs); 
		return *this; 
	}
	inline OPvec2& operator*(OPfloat& vhs) { 
		OPvec2 temp;
		OPvec2scl(&temp, this, vhs); 
		return temp; 
	}
	inline OPvec2& operator*=(OPfloat& vhs) { 
		OPvec2scl(this, this, vhs); 
		return *this; 
	}
	inline OPvec2& operator/(OPvec2& vhs) { 
		OPvec2 temp;
		OPvec2div(&temp, this, &vhs); 
		return temp; 
	}
	inline OPvec2& operator/=(OPvec2& vhs) { 
		OPvec2div(this, this, &vhs); 
		return *this; 
	}
};

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