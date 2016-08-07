#ifndef OP_MATH_VECTOR2
#define OP_MATH_VECTOR2

struct OPvec2;
typedef struct OPvec2 OPvec2;

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmath.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPstream.h"

//inline OPvec2 OPvec2(OPfloat x, OPfloat y);

inline void OPvec2Add(OPvec2* dst, OPvec2* a, OPvec2* b);
inline void OPvec2Sub(OPvec2* dst, OPvec2* a, OPvec2* b);
inline void OPvec2Mul(OPvec2* dst, OPvec2* a, OPvec2* b);
inline void OPvec2Scl(OPvec2* dst, OPvec2* a, OPfloat s);
inline void OPvec2Div(OPvec2* dst, OPvec2* a, OPvec2* b);
inline void OPvec2Div(OPvec2* dst, OPvec2* a, OPfloat b);

struct OPvec2 {
	union{
		struct {
			OPfloat x, y;
		};
		OPfloat row[2];
	};

	OPvec2() {
		x = 0; y = 0;
	}

	OPvec2(OPfloat x, OPfloat y) {
		this->x = x;
		this->y = y;
	}

	OPvec2 operator=(OPvec2 vhs) { 
		OPmemcpy(this, &vhs, sizeof(OPvec2)); return *this;
	}

	inline OPvec2 operator+=(OPfloat vhs) {
		this->x += vhs;
		this->y += vhs;
		return *this; 
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


	inline OPvec2 operator+(OPvec2 vhs) {
		OPvec2 temp(0, 0);
		OPvec2Add(&temp, this, &vhs);
		return temp;
	}
	inline OPvec2 operator-(OPvec2 vhs) {
		OPvec2 temp(0, 0);
		OPvec2Sub(&temp, this, &vhs);
		return temp;
	}
	inline OPvec2 operator*(OPvec2 vhs) {
		OPvec2 temp(0, 0);
		OPvec2Mul(&temp, this, &vhs);
		return temp;
	}
	inline OPvec2 operator*(OPfloat vhs) {
		OPvec2 temp(0, 0);
		OPvec2Scl(&temp, this, vhs);
		return temp;
	}
	//inline OPvec2 operator*(OPvec2 vhs) {
	//	OPvec2 temp(0, 0);
	//	OPvec2Scl(&temp, &vhs, this);
	//	return temp;
	//}
	inline OPvec2 operator/(OPvec2 vhs) {
		OPvec2 temp(0, 0);
		OPvec2Div(&temp, this, &vhs);
		return temp;
	}
	inline OPvec2 operator/(OPfloat vhs) {
		OPvec2 temp(0, 0);
		OPvec2Div(&temp, this, vhs);
		return temp;
	}

	//inline OPvec2 operator/(OPvec2 vhs) {
	//	OPvec2 temp(0, 0);
	//	OPvec2Div(&temp, &vhs, this);
	//	return temp;
	//}

	// Normalizes the array
	void Norm() {
		OPfloat l = (OPfloat)OPsqrt(this->x * this->x + this->y * this->y);
		if (l == 0) {
			return;
		}
		this->x /= l;
		this->y /= l;
	}
};

extern const OPvec2 OPVEC2_ZERO;
extern const OPvec2 OPVEC2_ONE;


//    ___            _         _ _        _       _ _   _              _   _    
//   / __|_  _ _ __ | |__  ___| (_)__    /_\  _ _(_) |_| |_  _ __  ___| |_(_)__ 
//   \__ \ || | '  \| '_ \/ _ \ | / _|  / _ \| '_| |  _| ' \| '  \/ -_)  _| / _|
//   |___/\_, |_|_|_|_.__/\___/_|_\__| /_/ \_\_| |_|\__|_||_|_|_|_\___|\__|_\__|
//        |__/                                                                  

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

//    ___             _   _               _     _       _ _   _              _   _    
//   | __|  _ _ _  __| |_(_)___ _ _  __ _| |   /_\  _ _(_) |_| |_  _ __  ___| |_(_)__ 
//   | _| || | ' \/ _|  _| / _ \ ' \/ _` | |  / _ \| '_| |  _| ' \| '  \/ -_)  _| / _|
//   |_| \_,_|_||_\__|\__|_\___/_||_\__,_|_| /_/ \_\_| |_|\__|_||_|_|_|_\___|\__|_\__|
//                                                                                    
inline OPfloat OPvec2Dot(OPvec2 a, OPvec2 b){
	return a.x * b.x + a.y * b.y;
}

inline OPfloat OPvec2Cross(OPvec2 a, OPvec2 b){
	return a.x * b.y - a.y * b.x;
}

inline OPfloat OPvec2Len(OPvec2 v){
	return (OPfloat)OPsqrt(v.x * v.x + v.y * v.y);
}

inline OPvec2 OPvec2Norm(OPvec2 a){
	OPfloat l = (OPfloat)OPsqrt(a.x * a.x + a.y * a.y);
	if(l == 0) {
		return a;
	}
	return OPvec2(a.x / l, a.y / l);
}

inline OPvec2 OPvec2Perp(OPvec2 a){
	return OPvec2(a.y, a.x);
}

inline OPfloat OPvec2Dist(OPvec2 a, OPvec2 b) {
	OPvec2 tmp = OPvec2(a.x - b.x, a.y - b.y);
	return OPvec2Len(tmp);
}

inline OPvec2 OPvec2Reflect(OPvec2 horizon, OPvec2 v){
	OPfloat ratio = 2 * (OPvec2Dot(v, horizon) / OPvec2Dot(horizon, horizon));
	return horizon * ratio - v;
}

inline OPfloat OPvec2Angle(OPvec2 a, OPvec2 b) {
	OPfloat dot = OPvec2Dot(a, b);
	OPfloat aLen = OPvec2Len(a);
	OPfloat bLen = OPvec2Len(b);
	return OPacos(dot / (aLen * bLen));
}

inline OPvec2 OPvec2Read(OPstream* str) {
	OPvec2 temp(
		OPreadf32(str),
		OPreadf32(str)
	);
	return temp;
}

inline OPvec2 OPvec2RandNorm(){
	OPvec2 v;
	v.x = OPrandom() - 0.5f;
	v.y = OPrandom() - 0.5f;
	return OPvec2Norm(v);
}

inline void OPvec2Write(OPvec2 v, OPstream* str) {
	OPwrite(str, &v.x, sizeof(f32));
	OPwrite(str, &v.y, sizeof(f32));
}

inline void OPvec2Log(const OPchar* m, OPvec2 v) {
	OPlog("%s: [%f, %f]", m, v.x, v.y);
}
#endif
