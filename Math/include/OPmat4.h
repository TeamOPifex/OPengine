#pragma once

struct OPmat4;
typedef struct OPmat4 OPmat4;

// INCLUDES
#include "./Math/include/OPvec2.h"
#include "./Math/include/OPvec3.h"
#include "./Math/include/OPvec4.h"
#include "./Math/include/OPquat.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPmath.h"
#include "./Core/include/OPlog.h"

// EXTERNALS
extern const OPmat4 OPMAT4_ZERO;
extern const OPmat4 OPMAT4_IDENTITY;

// METHOD PRE DECLARATIONS

// INLINE PRE DECLARATIONS
inline OPmat4 OPmat4Mul(OPmat4 m1, OPmat4 m2);
void OPmat4Mul(OPmat4* dst, OPmat4 m1, OPmat4 m2);

inline OPmat4 OPmat4RotX(OPfloat t);
inline void OPmat4RotX(OPmat4* m, OPfloat x);
inline OPmat4 OPmat4RotY(OPfloat t);
inline void OPmat4RotY(OPmat4* m, OPfloat x);
inline OPmat4 OPmat4RotZ(OPfloat t);
inline void OPmat4RotZ(OPmat4* m, OPfloat x);

inline OPmat4 OPmat4Translate(OPfloat x, OPfloat y, OPfloat z);
inline void OPmat4Translate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);

inline OPmat4 OPmat4Scl(OPfloat x);
inline OPmat4 OPmat4Scl(OPfloat x, OPfloat y, OPfloat z);
inline void OPmat4Scl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);

inline OPvec2 OPmat4Transform(OPvec2 a, OPmat4 b);
OPvec3 OPmat4Transform(OPvec3 a, OPmat4 b);
OPvec4 OPmat4Transform(OPvec4 a, OPmat4 b);
OPmat4 OPmat4From(OPquat a);

// STRUCT DEFINITIONS

// Data Structure 4 * 4 = 16 floats
//		32 bit = 16 * 32 = 512 bits or 64 bytes
//		64 bit = 16 * 64 = 1024 bits or 128 bytes
struct OPmat4 {
	OPvec4 cols[4];

	OPmat4 operator=(OPmat4 vhs) {
		OPmemcpy(this, (void*)&vhs, sizeof(OPmat4)); return *this;
	}
	inline OPvec4& operator[](const i32 i) {
		return cols[i];
	}

	inline OPmat4 operator*(OPmat4 rhs) {
		OPmat4 temp;
		OPmat4Mul(&temp, *this, rhs);
		return temp;
	}

	inline OPmat4& operator*=(OPmat4 vhs) {
		OPmat4Mul(this, *this, vhs);
		return *this;
	}
	inline OPmat4& operator/=(OPvec2 vhs) {
		OPmat4Scl(this, vhs.x, vhs.y, 1);
		return *this;
	}
	inline OPmat4& operator/=(OPvec3 vhs) {
		OPmat4Scl(this, vhs.x, vhs.y, vhs.z);
		return *this;
	}
	inline OPmat4& operator/=(OPvec4 vhs) {
		OPmat4Scl(this, vhs.x, vhs.y, vhs.z);
		return *this;
	}
	inline OPmat4& operator+=(OPvec2 vhs) {
		OPmat4Translate(this, vhs.x, vhs.y, 0);
		return *this;
	}
	inline OPmat4& operator+=(OPvec2* vhs) {
		OPmat4Translate(this, vhs->x, vhs->y, 0);
		return *this;
	}
	inline OPmat4& operator+=(OPvec3 vhs) {
		OPmat4Translate(this, vhs.x, vhs.y, vhs.z);
		return *this;
	}
	inline OPmat4& operator+=(OPvec3* vhs) {
		OPmat4Translate(this, vhs->x, vhs->y, vhs->z);
		return *this;
	}
	inline OPmat4& operator+=(OPvec4 vhs) {
		OPmat4Translate(this, vhs.x, vhs.y, vhs.z);
		return *this;
	}
	inline OPmat4& operator+=(OPvec4* vhs) {
		OPmat4Translate(this, vhs->x, vhs->y, vhs->z);
		return *this;
	}
	inline OPmat4& operator-=(OPvec2 vhs) {
		OPmat4Translate(this, -vhs.x, -vhs.y, 0);
		return *this;
	}
	inline OPmat4& operator-=(OPvec2* vhs) {
		OPmat4Translate(this, -vhs->x, -vhs->y, 0);
		return *this;
	}
	inline OPmat4& operator-=(OPvec3 vhs) {
		OPmat4Translate(this, -vhs.x, -vhs.y, -vhs.z);
		return *this;
	}
	inline OPmat4& operator-=(OPvec3* vhs) {
		OPmat4Translate(this, -vhs->x, -vhs->y, -vhs->z);
		return *this;
	}
	inline OPmat4& operator-=(OPvec4 vhs) {
		OPmat4Translate(this, -vhs.x, -vhs.y, -vhs.z);
		return *this;
	}
	inline OPmat4& operator-=(OPvec4* vhs) {
		OPmat4Translate(this, -vhs->x, -vhs->y, -vhs->z);
		return *this;
	}


	inline OPmat4* SetRotX(f32 val) {
		*this = OPmat4RotX(val);
		return this;
	}

	inline OPmat4* SetRotY(f32 val) {
		*this = OPmat4RotY(val);
		return this;
	}

	inline OPmat4* SetRotZ(f32 val) {
		*this = OPmat4RotZ(val);
		return this;
	}

	inline OPmat4* SetScl(f32 val) {
		*this = OPmat4Scl(val);
		return this;
	}

	inline OPmat4* SetScl(f32 x, f32 y, f32 z) {
		*this = OPmat4Scl(x, y, z);
		return this;
	}

	inline OPmat4* SetTranslate(OPvec3 val) {
		*this = OPmat4Translate(val.x, val.y, val.z);
		return this;
	}

	inline OPmat4* SetTranslate(f32 x, f32 y, f32 z) {
		*this = OPmat4Translate(x, y, z);
		return this;
	}

	inline OPmat4* SetIdentity() {
		*this = OPMAT4_IDENTITY;
		return this;
	}

	inline OPmat4* RotX(f32 val) {
		OPmat4RotX(this, val);
		return this;
	}

	inline OPmat4* RotY(f32 val) {
		OPmat4RotY(this, val);
		return this;
	}

	inline OPmat4* RotZ(f32 val) {
		OPmat4RotZ(this, val);
		return this;
	}

	inline OPmat4* Rot(OPquat val) {
		OPmat4Mul(this, *this, OPmat4From(val));
		return this;
	}

	inline OPmat4* Scl(f32 val) {
		OPmat4Scl(this, val, val, val);
		return this;
	}

	inline OPmat4* Scl(f32 x, f32 y, f32 z) {
		OPmat4Scl(this, x, y, z);
		return this;
	}

	inline OPmat4* Scl(OPvec3 scl) {
		OPmat4Scl(this, scl.x, scl.y, scl.z);
		return this;
	}

	inline OPmat4* Translate(f32 x, f32 y, f32 z) {
		OPmat4Translate(this, x, y, z);
		return this;
	}

	inline OPmat4* Translate(OPvec3 val) {
		OPmat4Translate(this,val.x, val.y,val.z);
		return this;
	}
	
	inline OPvec3 GetTranslation() {
		return OPvec3(
			cols[3][0],
			cols[3][1],
			cols[3][2]
		);
	}
};


inline OPmat4 OPmat4Create(
	f32 _00, f32 _10, f32 _20, f32 _30,
	f32 _01, f32 _11, f32 _21, f32 _31,
	f32 _02, f32 _12, f32 _22, f32 _32,
	f32 _03, f32 _13, f32 _23, f32 _33) {

	OPmat4 temp = {
		OPvec4(_00, _10, _20, _30),
		OPvec4(_01, _11, _21, _31),
		OPvec4(_02, _12, _22, _32),
		OPvec4(_03, _13, _23, _33)
	};

	return temp;
}

inline OPmat4 OPmat4Create(OPvec4 row0, OPvec4 row1, OPvec4 row2, OPvec4 row3) {
	return OPmat4Create(
		row0.x, row0.y, row0.z, row0.w,
		row1.x, row1.y, row1.z, row1.w,
		row2.x, row2.y, row2.z, row2.w,
		row3.x, row3.y, row3.z, row3.w
		);
}

void OPmat4Mul(OPmat4* dst, OPmat4 m1, OPmat4 m2);

inline void OPmat4Identity(OPmat4* m) {
	*m = OPMAT4_IDENTITY;
}


 inline OPmat4 OPmat4Transpose(OPmat4 m){
	OPmat4 tmp;
	for(i32 i = 0; i < 4; i++) {
		for(i32 j = 0; j < 4; j++) {
			tmp[j][i] = m[i][j];
		}
	}
	return tmp;
}

 inline OPvec3 OPmat4GetTranslate(OPmat4 mat) {
	 return OPvec3Create(mat[3][0], mat[3][1], mat[3][2]);
 }

 inline OPmat4 OPmat4RotX(OPfloat t) {
	OPfloat t1 = OPcos(t);
	OPfloat t2 = OPsin(t);
	OPmat4 m = OPMAT4_IDENTITY;
	m[1][1] = t1;
	m[1][2] = t2;
	m[2][1] = -t2;
	m[2][2] = t1;
	return m;
}

 inline OPmat4 OPmat4RotY(OPfloat t){
	OPfloat t1 = OPcos(t);
	OPfloat t2 = OPsin(t);
	OPmat4 m = OPMAT4_IDENTITY;
	m[0][0] = t1;
	m[0][2] = -t2;
	m[2][0] = t2;
	m[2][2] = t1;
	return m;
}

 inline OPmat4 OPmat4RotZ(OPfloat t) {
	OPfloat t1 = OPcos(t);
	OPfloat t2 = OPsin(t);
	OPmat4 m = OPMAT4_IDENTITY;
	m[0][0] = t1;
	m[0][1] = t2;
	m[1][0] = -t2;
	m[1][1] = t1;
	return m;
}

 inline OPmat4 OPmat4Translate(OPfloat x, OPfloat y, OPfloat z) {
	 OPmat4 m = OPMAT4_IDENTITY;
	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;
	return m;
}

 inline OPmat4 OPmat4Translate(OPvec3 v) {
	 OPmat4 m = OPMAT4_IDENTITY;
	m[3].x = v.x;
	m[3].y = v.y;
	m[3].z = v.z;
	return m;
}

 inline OPmat4 OPmat4Translate(OPvec2 v) {
	OPmat4 m = OPMAT4_IDENTITY;
	m[3].x = v.x;
	m[3].y = v.y;
	m[3].z = 0;
	return m;
}

 inline OPmat4 OPmat4Scl(OPfloat x, OPfloat y, OPfloat z) {
	 OPmat4 m = OPMAT4_IDENTITY;
	m[0][0] = x;
	m[1][1] = y;
	m[2][2] = z;
	return m;
 }

 inline OPmat4 OPmat4Scl(OPfloat x) {
	return OPmat4Scl(x, x, x);
 }

 inline OPmat4 OPmat4RotX(OPmat4 m, OPfloat x) {
	OPmat4 result = OPMAT4_ZERO;
	OPmat4 temp = OPmat4RotX(x);
	OPmat4Mul(&result, m, temp);
	return result;
 }

 inline OPmat4 OPmat4RotY(OPmat4 m, OPfloat x) {
	 OPmat4 result = OPMAT4_ZERO;
	 OPmat4 temp = OPmat4RotY(x);
	 OPmat4Mul(&result, m, temp);
	 return result;
 }

 inline OPmat4 OPmat4RotZ(OPmat4 m, OPfloat x) {
	 OPmat4 result = OPMAT4_ZERO;
	 OPmat4 temp = OPmat4RotZ(x);
	 OPmat4Mul(&result, m, temp);
	 return result;
 }

 inline OPmat4 OPmat4Scl(OPmat4 m, OPfloat x, OPfloat y, OPfloat z) {
	 OPmat4 result = OPMAT4_ZERO;
	 OPmat4 temp = OPmat4Scl(x, y, z);
	 OPmat4Mul(&result, m, temp);
	 return result;
 }

 inline OPmat4 OPmat4Scl(OPmat4 m, OPfloat x) {
	 return OPmat4Scl(m, x, x, x);
 }

  inline OPmat4 OPmat4Translate(OPmat4 m, OPfloat x, OPfloat y, OPfloat z) {
 	 m[3][0] += x;
 	 m[3][1] += y;
 	 m[3][2] += z;
 	 return m;
 }

  inline OPmat4 OPmat4SetTranslate(OPmat4 m, OPfloat x, OPfloat y, OPfloat z) {
 	 m[3][0] = x;
 	 m[3][1] = y;
 	 m[3][2] = z;
 	 return m;
 }

 inline OPmat4 OPmat4Translate(OPmat4 m, OPvec3 v) {
	 m[3][0] += v.x;
	 m[3][1] += v.y;
	 m[3][2] += v.z;
	 return m;
}

inline OPmat4 OPmat4SetTranslate(OPmat4 m, OPvec3 v) {
	 m[3][0] = v.x;
	 m[3][1] = v.y;
	 m[3][2] = v.z;
	 return m;
}


 inline void OPmat4RotX(OPmat4* m, OPfloat x) {
	 OPmat4Mul(m, *m, OPmat4RotX(x));
 }

 inline void OPmat4RotY(OPmat4* m, OPfloat x) {
	 OPmat4Mul(m, *m, OPmat4RotY(x));
 }

 inline void OPmat4RotZ(OPmat4* m, OPfloat x) {
	 OPmat4Mul(m, *m, OPmat4RotZ(x));
 }

 inline void OPmat4Translate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z){
	 OPmat4Mul(m, *m, OPmat4Translate(x, y, z));
 }

 inline void OPmat4Scl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z) {
	 OPmat4Mul(m, *m, OPmat4Scl(x, y, z));
 }

 inline OPfloat OPmat4GetCofactor(OPfloat m0, OPfloat m1, OPfloat m2,
	 OPfloat m3, OPfloat m4, OPfloat m5,
	 OPfloat m6, OPfloat m7, OPfloat m8)
 {
	 return m0 * (m4 * m8 - m5 * m7) -
		 m1 * (m3 * m8 - m5 * m6) +
		 m2 * (m3 * m7 - m4 * m6);
 }



 OPvec3 OPmat4Transform(OPvec3 a, OPmat4 b);
 OPvec4 OPmat4Transform(OPvec4 a, OPmat4 b);

OPmat4 OPmat4From(OPquat a);

void OPmat4Log(const OPchar* msg, OPmat4 m);
OPmat4 OPmat4Read(OPstream* str);
void OPmat4Write(OPmat4 v, OPstream* str);

OPmat4 OPmat4RemoveScale(OPmat4 a);
OPvec3 OPmat4Eulor(OPmat4 a);

OPmat4 OPmat4Ortho(OPfloat left, OPfloat right, OPfloat bottom, OPfloat top, OPfloat zNear, OPfloat zFar);
OPmat4 OPmat4LookAt(OPvec3 eye, OPvec3 at, OPvec3 up);
OPmat4 OPmat4Perspective(OPfloat fovy, OPfloat aspect, OPfloat nearVal, OPfloat farVal);
OPint OPmat4Inverse(OPmat4* dst, OPmat4 a);
OPmat4 OPmat4Interpolate(OPmat4 a, OPmat4 b, OPfloat percent);
OPmat4 OPmat4RotationBetween(OPvec3 start, OPvec3 dest);
OPmat4 OPmat4RotationNormal(OPvec3 normal);
OPmat4 OPmat4RotationNormal2(OPvec3 normal);

