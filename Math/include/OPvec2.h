#ifndef OP_MATH_VECTOR2
#define OP_MATH_VECTOR2
#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmath.h"
#include "./Core/include/OPmemory.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPstream.h"

struct OPvec2;
inline OPfloat* OPvec2Index(OPvec2* v, int idx);
inline OPvec2 OPvec2Create(OPfloat x, OPfloat y);
inline void OPvec2Add(OPvec2* dst, OPvec2* a, OPvec2* b);
inline void OPvec2Sub(OPvec2* dst, OPvec2* a, OPvec2* b);
inline void OPvec2Mul(OPvec2* dst, OPvec2* a, OPvec2* b);
inline void OPvec2Scl(OPvec2* dst, OPvec2* a, OPfloat s);
inline void OPvec2Div(OPvec2* dst, OPvec2* a, OPvec2* b);
inline void OPvec2Div(OPvec2* dst, OPvec2* a, OPfloat b);
inline void OPvec2Dot(OPfloat* dst, OPvec2* a, OPvec2* b);
inline void OPvec2Cross(OPfloat* dst, OPvec2* a, OPvec2* b);
inline void OPvec2Len(OPfloat* dst, OPvec2* v);
inline void OPvec2Norm(OPvec2* dst, OPvec2* a);
inline void OPvec2Perp(OPvec2* dst, OPvec2* a);
inline void OPvec2Dist(OPfloat* dst, OPvec2* a, OPvec2* b);

inline OPvec2 OPvec2Add(OPvec2* a, OPvec2* b);
inline OPvec2 OPvec2Sub(OPvec2* a, OPvec2* b);
inline OPvec2 OPvec2Mul(OPvec2* a, OPvec2* b);
inline OPvec2 OPvec2Scl(OPvec2* a, OPfloat s);
inline OPvec2 OPvec2Div(OPvec2* a, OPvec2* b);
inline OPvec2 OPvec2Div(OPvec2* a, OPfloat b);
inline OPfloat OPvec2Dot(OPvec2* a, OPvec2* b);
inline OPfloat OPvec2Len(OPvec2* v);
inline OPvec2 OPvec2Norm(OPvec2* a);
inline OPvec2 OPvec2Perp(OPvec2* a);
inline OPfloat OPvec2Dist( OPvec2* a, OPvec2* b);
inline OPfloat OPvec2Angle(OPvec2* a, OPvec2* b);

struct OPvec2 {
	union{
		struct {
			OPfloat x, y;
		};
		OPfloat row[2];
	};

	OPvec2 operator=(OPvec2 vhs) { 
		OPmemcpy(this, &vhs, sizeof(OPvec2)); return *this;
	}
	inline OPvec2 operator+=(OPvec2 vhs) { 
		OPvec2Add(this, this, &vhs); 
		return *this; 
	}
	inline OPvec2 operator-=(OPvec2 vhs) { 
		OPvec2Sub(this, this, &vhs); 
		return *this; 
	}
	inline OPvec2 operator*=(OPvec2 vhs) { 
		OPvec2Mul(this, this, &vhs); 
		return *this; 
	}
	inline OPvec2 operator*=(OPfloat vhs) { 
		OPvec2Scl(this, this, vhs); 
		return *this; 
	}
	inline OPvec2 operator/=(OPvec2 vhs) { 
		OPvec2Div(this, this, &vhs); 
		return *this; 
	}
	inline OPvec2 operator/=(OPfloat vhs) { 
		OPvec2Div(this, this, vhs); 
		return *this; 
	}
	inline OPfloat& operator[](i32 i) {
		return row[i];
	}
};

extern const OPvec2 OPvec2Zero;
extern const OPvec2 OPvec2One;

inline OPvec2 OPvec2Create(OPfloat x, OPfloat y) {
	OPvec2 tmp = { x, y };
	return tmp;
}

inline OPvec2 operator+(OPvec2 lhs, OPvec2 vhs) { 
	OPvec2 temp = { 0.0, 0.0 };
	OPvec2Add(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec2 operator-(OPvec2 lhs, OPvec2 vhs) { 
	OPvec2 temp = { 0.0, 0.0 };
	OPvec2Sub(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec2 operator*(OPvec2 lhs, OPvec2 vhs) { 
	OPvec2 temp = { 0.0, 0.0 };
	OPvec2Mul(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec2 operator*(OPvec2 lhs, OPfloat vhs) { 
	OPvec2 temp = { 0.0, 0.0 };
	OPvec2Scl(&temp, &lhs, vhs); 
	return temp; 
}
inline OPvec2 operator*(OPfloat lhs, OPvec2 vhs) { 
	OPvec2 temp = { 0.0, 0.0 };
	OPvec2Scl(&temp, &vhs, lhs); 
	return temp; 
}
inline OPvec2 operator/(OPvec2 lhs, OPvec2 vhs) { 
	OPvec2 temp = { 0.0, 0.0 };
	OPvec2Div(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec2 operator/(OPvec2 lhs, OPfloat vhs) { 
	OPvec2 temp = { 0.0, 0.0 };
	OPvec2Div(&temp, &lhs, vhs); 
	return temp; 
}

inline OPvec2 operator/(OPfloat lhs, OPvec2 vhs) { 
	OPvec2 temp = { 0.0, 0.0 };
	OPvec2Div(&temp, &vhs, lhs); 
	return temp; 
}


inline OPfloat* OPvec2Index(OPvec2* v, int idx){
	return &((OPfloat*)(v))[idx];
}

inline void OPvec2Add(OPvec2* dst, OPvec2* a, OPvec2* b){
	dst->x = a->x + b->x;
	dst->y = a->y + b->y;
}

inline void OPvec2Sub(OPvec2* dst, OPvec2* a, OPvec2* b){
	dst->x = a->x - b->x;
	dst->y = a->y - b->y;
}

inline void OPvec2Mul(OPvec2* dst, OPvec2* a, OPvec2* b){
	dst->x = a->x * b->x;
	dst->y = a->y * b->y;
}

inline void OPvec2Scl(OPvec2* dst, OPvec2* a, OPfloat s){
	dst->x = a->x * s;
	dst->y = a->y * s;
}

inline void OPvec2Div(OPvec2* dst, OPvec2* a, OPvec2* b){
	dst->x = a->x / b->x;
	dst->y = a->y / b->y;
}
inline void OPvec2Div(OPvec2* dst, OPvec2* a, OPfloat b){
	dst->x = a->x / b;
	dst->y = a->y / b;
}

inline void OPvec2Dot(OPfloat* dst, OPvec2* a, OPvec2* b){
	(*dst) = a->x * b->x + a->y * b->y;
}

inline void OPvec2Cross(OPfloat* dst, OPvec2* a, OPvec2* b){
	(*dst) = a->x * b->y - a->y * b->x;
}

inline void OPvec2Len(OPfloat* dst, OPvec2* v){
	(*dst) = OPsqrt(v->x * v->x + v->y * v->y);
}

inline void OPvec2Norm(OPvec2* dst, OPvec2* a){
	OPfloat l = OPsqrt(a->x * a->x + a->y * a->y);
	if(l == 0) {
		return;	
	}
	dst->x = a->x / l;
	dst->y = a->y / l;
}

inline void OPvec2Perp(OPvec2* dst, OPvec2* a){
	dst->x = a->y;
	dst->y = a->x;
}

inline void OPvec2Dist(OPfloat* dst, OPvec2* a, OPvec2* b) {
	OPvec2 tmp;
	tmp.x = a->x - b->x;
	tmp.y = a->y - b->y;
	OPvec2Len(dst, &tmp);
}


inline OPvec2 OPvec2Add(OPvec2* a, OPvec2* b) { 
	OPvec2 temp;
	OPvec2Add(&temp, a, b);
	return temp;
}

inline OPvec2 OPvec2Sub(OPvec2* a, OPvec2* b) { 
	OPvec2 temp;
	OPvec2Add(&temp, a, b);
	return temp;
}

inline OPvec2 OPvec2Mul(OPvec2* a, OPvec2* b) { 
	OPvec2 temp;
	OPvec2Mul(&temp, a, b);
	return temp;
}

inline OPvec2 OPvec2Scl(OPvec2* a, OPfloat s) { 
	OPvec2 temp;
	OPvec2Scl(&temp, a, s);
	return temp;
}

inline OPvec2 OPvec2Div(OPvec2* a, OPvec2* b) { 
	OPvec2 temp;
	OPvec2Div(&temp, a, b);
	return temp;
}

inline OPvec2 OPvec2Div(OPvec2* a, OPfloat b) { 
	OPvec2 temp;
	OPvec2Div(&temp, a, b);
	return temp;
}

inline OPfloat OPvec2Dot(OPvec2* a, OPvec2* b) { 
	OPfloat temp;
	OPvec2Dot(&temp, a, b);
	return temp;
}

inline OPfloat OPvec2Len(OPvec2* v) { 
	OPfloat temp;
	OPvec2Len(&temp, v);
	return temp;
}

inline OPvec2 OPvec2Norm(OPvec2* a) { 
	OPvec2 temp;
	OPvec2Norm(&temp, a);
	return temp;
}

inline OPvec2 OPvec2Perp(OPvec2* a) { 
	OPvec2 temp;
	OPvec2Perp(&temp, a);
	return temp;
}

inline OPvec2 OPvec2reflect(OPvec2* horizon, OPvec2* v){
	OPfloat ratio = 2 * (OPvec2Dot(v, horizon) / OPvec2Dot(horizon, horizon));
	return *horizon * ratio - *v;
}

inline OPfloat OPvec2Dist(OPvec2* a, OPvec2* b) {
	OPfloat tmp;
	OPvec2Dist(&tmp, a, b);
	return tmp;
}


inline OPfloat OPvec2Angle(OPvec2* a, OPvec2* b) {
	OPfloat tmp;
	OPfloat dot;
	OPvec2Dot(&dot, a, b);
	OPfloat aLen, bLen;
	OPvec2Len(&aLen, a);
	OPvec2Len(&bLen, b);
	tmp = OPacos(dot / (aLen * bLen));
	return tmp;
}

inline OPvec2 OPvec2str(OPstream* str) {
	OPvec2 temp = {
		OPreadf32(str),
		OPreadf32(str)
	};
	return temp;
}

inline void OPvec2write(OPvec2* v, OPstream* str) {
	OPwrite(str, &v->x, sizeof(f32));
	OPwrite(str, &v->y, sizeof(f32));
}
#endif
