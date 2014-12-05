#ifndef OP_MATH_VECTOR2
#define OP_MATH_VECTOR2
#include "./Core/include/Types.h"
#include "./Core/include/MathHelpers.h"
#include "./Core/include/DynamicMemory.h"
#include "./Data/include/File.h"
#include "./Data/include/Stream.h"

struct OPvec2;
inline OPfloat* OPvec2index(OPvec2* v, int idx);
inline OPvec2 OPvec2create(OPfloat x, OPfloat y);
inline void OPvec2add(OPvec2* dst, OPvec2* a, OPvec2* b);
inline void OPvec2sub(OPvec2* dst, OPvec2* a, OPvec2* b);
inline void OPvec2mul(OPvec2* dst, OPvec2* a, OPvec2* b);
inline void OPvec2scl(OPvec2* dst, OPvec2* a, OPfloat s);
inline void OPvec2div(OPvec2* dst, OPvec2* a, OPvec2* b);
inline void OPvec2divf(OPvec2* dst, OPvec2* a, OPfloat b);
inline void OPvec2dot(OPfloat* dst, OPvec2* a, OPvec2* b);
inline void OPvec2cross(OPfloat* dst, OPvec2* a, OPvec2* b);
inline void OPvec2len(OPfloat* dst, OPvec2* v);
inline void OPvec2norm(OPvec2* dst, OPvec2* a);
inline void OPvec2perp(OPvec2* dst, OPvec2* a);
inline void OPvec2dist(OPfloat* dst, OPvec2* a, OPvec2* b);

inline OPvec2 OPvec2add(OPvec2* a, OPvec2* b);
inline OPvec2 OPvec2sub(OPvec2* a, OPvec2* b);
inline OPvec2 OPvec2mul(OPvec2* a, OPvec2* b);
inline OPvec2 OPvec2scl(OPvec2* a, OPfloat s);
inline OPvec2 OPvec2div(OPvec2* a, OPvec2* b);
inline OPvec2 OPvec2divf(OPvec2* a, OPfloat b);
inline OPfloat OPvec2dot(OPvec2* a, OPvec2* b);
inline OPfloat OPvec2len(OPvec2* v);
inline OPvec2 OPvec2norm(OPvec2* a);
inline OPvec2 OPvec2perp(OPvec2* a);
inline OPfloat OPvec2dist( OPvec2* a, OPvec2* b);
inline OPfloat OPvec2angle(OPvec2* a, OPvec2* b);

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
	inline OPfloat& operator[](i32 i) {
		return row[i];
	}
};

extern const OPvec2 OPvec2Zero;
extern const OPvec2 OPvec2One;

inline OPvec2 OPvec2create(OPfloat x, OPfloat y) {
	OPvec2 tmp = { x, y };
	return tmp;
}

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

inline void OPvec2cross(OPfloat* dst, OPvec2* a, OPvec2* b){
	(*dst) = a->x * b->y - a->y * b->x;
}

inline void OPvec2len(OPfloat* dst, OPvec2* v){
	(*dst) = OPsqrt(v->x * v->x + v->y * v->y);
}

inline void OPvec2norm(OPvec2* dst, OPvec2* a){
	OPfloat l = OPsqrt(a->x * a->x + a->y * a->y);
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

inline void OPvec2dist(OPfloat* dst, OPvec2* a, OPvec2* b) {
	OPvec2 tmp;
	tmp.x = a->x - b->x;
	tmp.y = a->y - b->y;
	OPvec2len(dst, &tmp);
}


inline OPvec2 OPvec2add(OPvec2* a, OPvec2* b) { 
	OPvec2 temp;
	OPvec2add(&temp, a, b);
	return temp;
}

inline OPvec2 OPvec2sub(OPvec2* a, OPvec2* b) { 
	OPvec2 temp;
	OPvec2add(&temp, a, b);
	return temp;
}

inline OPvec2 OPvec2mul(OPvec2* a, OPvec2* b) { 
	OPvec2 temp;
	OPvec2mul(&temp, a, b);
	return temp;
}

inline OPvec2 OPvec2scl(OPvec2* a, OPfloat s) { 
	OPvec2 temp;
	OPvec2scl(&temp, a, s);
	return temp;
}

inline OPvec2 OPvec2div(OPvec2* a, OPvec2* b) { 
	OPvec2 temp;
	OPvec2div(&temp, a, b);
	return temp;
}

inline OPvec2 OPvec2divf(OPvec2* a, OPfloat b) { 
	OPvec2 temp;
	OPvec2divf(&temp, a, b);
	return temp;
}

inline OPfloat OPvec2dot(OPvec2* a, OPvec2* b) { 
	OPfloat temp;
	OPvec2dot(&temp, a, b);
	return temp;
}

inline OPfloat OPvec2len(OPvec2* v) { 
	OPfloat temp;
	OPvec2len(&temp, v);
	return temp;
}

inline OPvec2 OPvec2norm(OPvec2* a) { 
	OPvec2 temp;
	OPvec2norm(&temp, a);
	return temp;
}

inline OPvec2 OPvec2perp(OPvec2* a) { 
	OPvec2 temp;
	OPvec2perp(&temp, a);
	return temp;
}

inline OPvec2 OPvec2reflect(OPvec2* horizon, OPvec2* v){
	OPfloat ratio = 2 * (OPvec2dot(v, horizon) / OPvec2dot(horizon, horizon));
	return *horizon * ratio - *v;
}

inline OPfloat OPvec2dist(OPvec2* a, OPvec2* b) {
	OPfloat tmp;
	OPvec2dist(&tmp, a, b);
	return tmp;
}


inline OPfloat OPvec2angle(OPvec2* a, OPvec2* b) {
	OPfloat tmp;
	OPfloat dot;
	OPvec2dot(&dot, a, b);
	OPfloat aLen, bLen;
	OPvec2len(&aLen, a);
	OPvec2len(&bLen, b);
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
