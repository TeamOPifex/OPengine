#ifndef OP_MATH_VECTOR2
#define OP_MATH_VECTOR2
#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmath.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPstream.h"

struct OPvec2;
inline OPvec2 OPvec2Create(OPfloat x, OPfloat y);

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

extern IMPORT_METHOD const OPvec2 OPvec2Zero;
extern IMPORT_METHOD const OPvec2 OPvec2One;

inline OPvec2 OPvec2Create(OPfloat x, OPfloat y) {
    OPvec2 tmp;
    tmp.x = x; tmp.y = y;
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
	return OPsqrt(v.x * v.x + v.y * v.y);
}

inline OPvec2 OPvec2Norm(OPvec2 a){
	OPfloat l = OPsqrt(a.x * a.x + a.y * a.y);
	if(l == 0) {
		return a;
	}
	return OPvec2Create(a.x / l, a.y / l);
}

inline OPvec2 OPvec2Perp(OPvec2 a){
	return OPvec2Create(a.y, a.x);
}

inline OPfloat OPvec2Dist(OPvec2 a, OPvec2 b) {
	OPvec2 tmp = OPvec2Create(a.x - b.x, a.y - b.y);
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
	OPvec2 temp = {
		OPreadf32(str),
		OPreadf32(str)
	};
	return temp;
}

inline void OPvec2Write(OPvec2 v, OPstream* str) {
	OPwrite(str, &v.x, sizeof(f32));
	OPwrite(str, &v.y, sizeof(f32));
}

inline void OPvec2Log(const OPchar* m, OPvec2 v) {
	OPlog("%s: [%f, %f]", m, v.x, v.y);
}
#endif
