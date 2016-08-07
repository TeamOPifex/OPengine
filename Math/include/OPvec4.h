#ifndef OP_MATH_VECTOR4
#define OP_MATH_VECTOR4

struct OPvec4;

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPmath.h"
#include "./Math/include/OPvec3.h"

OPvec4 OPvec4Create(OPfloat x, OPfloat y, OPfloat z, OPfloat w);

void OPvec4Add(OPvec4* dst, OPvec4* a, OPvec4* b);
void OPvec4Sub(OPvec4* dst, OPvec4* a, OPvec4* b);
void OPvec4Mul(OPvec4* dst, OPvec4* a, OPvec4* b);
void OPvec4Scl(OPvec4* dst, OPvec4* a, OPfloat s);
void OPvec4Div(OPvec4* dst, OPvec4* a, OPvec4* b);
void OPvec4Div(OPvec4* dst, OPvec4* a, OPfloat b);

struct OPvec4 {
	union{
		struct {
			OPfloat x, y, z, w;
		};
		OPfloat row[4];
	};
	OPvec4() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	}
	OPvec4(OPfloat x) {
		this->x = x;
		this->y = x;
		this->z = x;
		this->w = x;
	}
	OPvec4(OPfloat x, OPfloat y, OPfloat z, OPfloat w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

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


	inline OPvec4 operator+(OPvec4 vhs) {
		OPvec4 temp;
		OPvec4Add(&temp, this, &vhs);
		return temp;
	}
	inline OPvec4 operator-(OPvec4 vhs) {
		OPvec4 temp;
		OPvec4Sub(&temp, this, &vhs);
		return temp;
	}
	inline OPvec4 operator*(OPvec4 vhs) {
		OPvec4 temp;
		OPvec4Mul(&temp, this, &vhs);
		return temp;
	}
	inline OPvec4 operator*(OPfloat vhs) {
		OPvec4 temp(0);
		OPvec4Scl(&temp, this, vhs);
		return temp;
	}
	//inline OPvec4 operator*(OPvec4 vhs) {
	//	OPvec4 temp = { 0, 0, 0 };
	//	OPvec4Scl(&temp, this, vhs);
	//	return temp;
	//}
	inline OPvec4 operator/(OPvec4 vhs) {
		OPvec4 temp;
		OPvec4Div(&temp, this, &vhs);
		return temp;
	}
	inline OPvec4 operator/(OPfloat vhs) {
		OPvec4 temp;
		OPvec4Div(&temp, this, vhs);
		return temp;
	}
	//inline OPvec4 operator/(OPvec4 vhs) {
	//	OPvec4 temp = { 0, 0, 0 };
	//	OPvec4Div(&temp, this, lhs);
	//	return temp;
	//}
};

extern const OPvec4 OPVEC4_ZERO;
extern const OPvec4 OPVEC4_ONE;

inline OPvec4 OPvec4Create(OPfloat x, OPfloat y, OPfloat z, OPfloat w) {
	OPvec4 tmp(x, y, z, w);
	return tmp;
}

inline OPvec4 OPvec4Create(OPfloat x) {
	OPvec4 tmp(x, x, x, x);
	return tmp;
}

inline OPvec4 OPvec4Create(OPvec3 xyz, OPfloat w) {
	OPvec4 tmp(xyz.x, xyz.y, xyz.z, w);
	return tmp;
}

//    ___            _         _ _        _       _ _   _              _   _
//   / __|_  _ _ __ | |__  ___| (_)__    /_\  _ _(_) |_| |_  _ __  ___| |_(_)__
//   \__ \ || | '  \| '_ \/ _ \ | / _|  / _ \| '_| |  _| ' \| '  \/ -_)  _| / _|
//   |___/\_, |_|_|_|_.__/\___/_|_\__| /_/ \_\_| |_|\__|_||_|_|_|_\___|\__|_\__|
//        |__/


inline OPvec4 OPvec4Norm(OPvec4 a) {
	OPfloat l = (OPfloat)OPsqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
	return OPvec4Create(
		a.x / l,
		a.y / l,
		a.z / l,
		a.w / l
		);
}

inline OPfloat OPvec4Dot(OPvec4 a, OPvec4 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z+ a.w * b.w;
}

inline OPfloat OPvec4Len(OPvec4 a) {
	return (OPfloat)OPsqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

inline OPfloat OPvec4Dist(OPvec4 a, OPvec4 b) {
	OPvec4 tmp = OPvec4Create( a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
	return OPvec4Len(tmp);
}

inline OPvec4 OPvec4Read(OPstream* str) {
	OPvec4 temp(OPreadf32(str),
		OPreadf32(str),
		OPreadf32(str),
		OPreadf32(str));
	return temp;
}

inline void OPvec4Write(OPvec4 v, OPstream* str) {
	OPwrite(str, &v.x, sizeof(f32));
	OPwrite(str, &v.y, sizeof(f32));
	OPwrite(str, &v.z, sizeof(f32));
	OPwrite(str, &v.w, sizeof(f32));
}

inline OPvec4 OPvec4randNorm(){
	OPvec4 v(OPrandom() - 0.5f,
		OPrandom() - 0.5f,
		OPrandom() - 0.5f,
		OPrandom() - 0.5f);

	return OPvec4Norm(v);
}

inline void OPvec4Log(const OPchar* m, OPvec4 v) {
	OPlog("%s: [%f, %f, %f, %f]", m, v.x, v.y, v.z, v.w);
}

inline void OPvec4Sub(OPvec4* dst, OPvec4* a, OPvec4* b) {
	dst->x = a->x - b->x;
	dst->y = a->y - b->y;
	dst->z = a->z - b->z;
	dst->w = a->w - b->w;
}

inline void OPvec4Div(OPvec4* dst, OPvec4* a, OPvec4* b) {
	dst->x = a->x / b->x;
	dst->y = a->y / b->y;
	dst->z = a->z / b->z;
	dst->w = a->w / b->w;
}
inline void OPvec4Div(OPvec4* dst, OPvec4* a, OPfloat b) {
	dst->x = a->x / b;
	dst->y = a->y / b;
	dst->z = a->z / b;
	dst->w = a->w / b;
}

#endif
