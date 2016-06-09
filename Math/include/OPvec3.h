#ifndef OP_MATH_VECTOR3
#define OP_MATH_VECTOR3
#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmath.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"
#include "./Data/include/OPstream.h"
#include "./Data/include/OPfile.h"
#include "./Math/include/OPvec2.h"

struct OPvec3;
struct OPmat4;

inline OPvec3 OPvec3Create(OPfloat x, OPfloat y, OPfloat z);

inline void OPvec3Add(OPvec3* dst, OPvec3* a, OPvec3* b);
inline void OPvec3Sub(OPvec3* dst, OPvec3* a, OPvec3* b);
inline void OPvec3Mul(OPvec3* dst, OPvec3* a, OPvec3* b);
inline void OPvec3Scl(OPvec3* dst, OPvec3* a, OPfloat s);
inline void OPvec3Div(OPvec3* dst, OPvec3* a, OPvec3* b);
inline void OPvec3Div(OPvec3* dst, OPvec3* a, OPfloat b);

OPfloat OPvec3AngleToTarget(OPvec3 pos, OPvec3 facing, OPvec3 target);

struct OPvec3 {
	union{
		struct { OPfloat x, y, z; };
		OPfloat row[3];
	};

	OPvec3() {
		x = 0; y = 0; z = 0;
	}

	OPvec3(OPfloat x) {
		this->x = x;
		this->y = x;
		this->z = x;
	}

	OPvec3(OPfloat x, OPfloat y, OPfloat z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline OPvec3 operator=(OPvec3 vhs) { 
		OPmemcpy(this, (void*)&vhs, sizeof(OPvec3)); return *this;
	}
	inline OPvec3 operator+=(OPvec3 vhs) { 
		OPvec3Add(this, this, &vhs); 
		return *this; 
	}
	inline OPvec3 operator-=(OPvec3 vhs) { 
		OPvec3Sub(this, this, &vhs); 
		return *this; 
	}
	OPvec3 operator*=(OPmat4 rhs);
	inline OPvec3 operator*=(OPvec3 vhs) { 
		OPvec3Mul(this, this, &vhs); 
		return *this; 
	}
	inline OPvec3 operator*=(OPfloat vhs) { 
		OPvec3Scl(this, this, vhs); 
		return *this; 
	}
	inline OPvec3 operator/=(OPvec3 vhs) { 
		OPvec3Div(this, this, &vhs); 
		return *this; 
	}
	inline OPvec3 operator/=(OPfloat vhs) { 
		OPvec3Div(this, this, vhs); 
		return *this; 
	}
	inline OPfloat& operator[](i32 i) {
		return row[i];
	}
	
	// Normalizes the array
	void Norm() {
		OPfloat l = (OPfloat)OPsqrt(this->x * this->x + this->y * this->y + this->z * this->z);
		if (l == 0) {
			return;
		}
		this->x /= l;
		this->y /= l;
		this->z /= l;
	}
};

extern const OPvec3 OPVEC3_ZERO;
extern const OPvec3 OPVEC3_ONE;
extern const OPvec3 OPVEC3_UP;
extern const OPvec3 OPVEC3_LEFT;
extern const OPvec3 OPVEC3_FORWARD;
extern const OPvec3 OPVEC3_BACKWARD;

inline OPvec3 OPvec3Create(OPfloat x, OPfloat y, OPfloat z) {
	OPvec3 tmp(x, y, z);
	return tmp;
}
inline OPvec3 OPvec3Create(OPvec2 xy, OPfloat z) {
	OPvec3 tmp(xy.x, xy.y, z);
	return tmp;
}
inline OPvec3 OPvec3Create(OPfloat x) {
	OPvec3 tmp(x, x, x);
	return tmp;
}

inline OPvec3 operator+(OPvec3 lhs, OPvec3 vhs) { 
	OPvec3 temp(0, 0, 0);
	OPvec3Add(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec3 operator-(OPvec3 lhs, OPvec3 vhs) { 
	OPvec3 temp(0, 0, 0);
	OPvec3Sub(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec3 operator*(OPvec3 lhs, OPvec3 vhs) { 
	OPvec3 temp(0, 0, 0);
	OPvec3Mul(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec3 operator*(OPvec3 lhs, OPfloat vhs) { 
	OPvec3 temp(0, 0, 0);
	OPvec3Scl(&temp, &lhs, vhs); 
	return temp; 
}
inline OPvec3 operator*(OPfloat lhs, OPvec3 vhs) { 
	OPvec3 temp(0, 0, 0);
	OPvec3Scl(&temp, &vhs, lhs); 
	return temp; 
}
inline OPvec3 operator/(OPvec3 lhs, OPvec3 vhs) { 
	OPvec3 temp(0, 0, 0);
	OPvec3Div(&temp, &lhs, &vhs); 
	return temp; 
}
inline OPvec3 operator/(OPvec3 lhs, OPfloat vhs) { 
	OPvec3 temp(0, 0, 0);
	OPvec3Div(&temp, &lhs, vhs); 
	return temp; 
}
inline OPvec3 operator/(OPfloat lhs, OPvec3 vhs) {
	OPvec3 temp(0, 0, 0);
	OPvec3Div(&temp, &vhs, lhs);
	return temp;
}

inline bool operator==(const OPvec3& lhs, const OPvec3& vhs) {
	return lhs.x == vhs.x && lhs.y == vhs.y && lhs.z == vhs.z;
}

//    ___            _         _ _        _       _ _   _              _   _    
//   / __|_  _ _ __ | |__  ___| (_)__    /_\  _ _(_) |_| |_  _ __  ___| |_(_)__ 
//   \__ \ || | '  \| '_ \/ _ \ | / _|  / _ \| '_| |  _| ' \| '  \/ -_)  _| / _|
//   |___/\_, |_|_|_|_.__/\___/_|_\__| /_/ \_\_| |_|\__|_||_|_|_|_\___|\__|_\__|
//        |__/                                                                  
inline void OPvec3Add(OPvec3* dst, OPvec3* a, OPvec3* b) {
	dst->x = a->x + b->x;
	dst->y = a->y + b->y;
	dst->z = a->z + b->z;
}

inline void OPvec3Sub(OPvec3* dst, OPvec3* a, OPvec3* b) {
	dst->x = a->x - b->x;
	dst->y = a->y - b->y;
	dst->z = a->z - b->z;
}

inline void OPvec3Mul(OPvec3* dst, OPvec3* a, OPvec3* b) {
	dst->x = a->x * b->x;
	dst->y = a->y * b->y;
	dst->z = a->z * b->z;
}

inline void OPvec3Scl(OPvec3* dst, OPvec3* a, OPfloat s) {
	dst->x = a->x * s;
	dst->y = a->y *s;
	dst->z = a->z *s;
}

inline void OPvec3Div(OPvec3* dst, OPvec3* a, OPvec3* b) {
	dst->x = a->x / b->x;
	dst->y = a->y / b->y;
	dst->z = a->z / b->z;
}

inline void OPvec3Div(OPvec3* dst, OPvec3* a, OPfloat b) {
	dst->x = a->x / b;
	dst->y = a->y / b;
	dst->z = a->z / b;
}


//    ___             _   _               _     _       _ _   _              _   _    
//   | __|  _ _ _  __| |_(_)___ _ _  __ _| |   /_\  _ _(_) |_| |_  _ __  ___| |_(_)__ 
//   | _| || | ' \/ _|  _| / _ \ ' \/ _` | |  / _ \| '_| |  _| ' \| '  \/ -_)  _| / _|
//   |_| \_,_|_||_\__|\__|_\___/_||_\__,_|_| /_/ \_\_| |_|\__|_||_|_|_|_\___|\__|_\__|
//                                                                                    
inline OPfloat OPvec3Dot(OPvec3 a, OPvec3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline OPvec3 OPvec3Cross(OPvec3 a, OPvec3 b) {
	OPvec3 v = {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x,
	};
	return v;
}

inline OPfloat OPvec3Len(OPvec3 v) {
	return (OPfloat)OPsqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline OPfloat OPvec3Dist(OPvec3 a, OPvec3 b) {
	OPvec3 dif = a - b;
	return OPvec3Len(dif);
}

inline OPfloat OPvec3DistSquare(OPvec3 a, OPvec3 b) {
	OPvec3 dif = a - b;
	return OPvec3Dot(dif, dif);
}

inline OPfloat OPvec3Angle(OPvec3 a, OPvec3 b) {
	OPfloat temp = 0;

	//OPfloat top = a.x * b.x + a.y * b.y + a.z * b.z;
	//OPfloat bottom = (OPfloat)OPsqrt(a.x * a.x + a.y * a.y + a.z * a.z) *
	//	(OPfloat)OPsqrt(b.x * b.x + b.y * b.y + b.z * b.z);
	//if(bottom == 0) return 0;
	//
	//OPfloat cosTheta = top / bottom;
	//temp = OPacos(cosTheta);

	OPfloat top = OPvec3Dot(a, b);
	OPfloat bottom = OPvec3Len(a) * OPvec3Len(b);
	if (bottom == 0) return 0;

	temp = OPacos(top / bottom);

	return temp;
}

inline OPvec3 OPvec3Reflect(OPvec3 v, OPvec3 n){
	return v - n * (2 * OPvec3Dot(v, n));
}

inline OPvec3 OPvec3Norm(OPvec3 v){
	return v / OPvec3Len(v);
}

inline OPvec3 OPvec3Abs(OPvec3 v){
	return OPvec3Create(OPabs(v.x), OPabs(v.y), OPabs(v.z));
}

inline OPvec3 OPvec3Read(OPstream* str) {
	OPvec3 temp(
		OPreadf32(str),
		OPreadf32(str),
		OPreadf32(str));
	return temp;
}

inline void OPvec3Write(OPvec3 v, OPstream* str) {
	OPwrite(str, &v.x, sizeof(f32));
	OPwrite(str, &v.y, sizeof(f32));
	OPwrite(str, &v.z, sizeof(f32));
}

inline OPvec3 OPvec3RandNorm(){
	OPvec3 v(OPrandom() - 0.5f,
		OPrandom() - 0.5f,
		OPrandom() - 0.5f);

	return OPvec3Norm(v);
}

inline void OPvec3Log(const OPchar* m, OPvec3 v) {
	OPlog("%s: [%f, %f, %f]", m, v.x, v.y, v.z);
}

inline OPvec3 OPvec3Tween(OPvec3 a, OPvec3 b, OPfloat delta) {
	return OPvec3Create(
		a.x + (b.x - a.x) * delta,
		a.y + (b.y - a.y) * delta,
		a.z + (b.z - a.z) * delta
		);
}
#endif