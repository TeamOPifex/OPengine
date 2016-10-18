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
inline OPvec3 OPmat4Transform(OPvec3 a, OPmat4 b);
inline OPvec4 OPmat4Transform(OPvec4 a, OPmat4 b);
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

 inline OPvec3 OPmat4Transform(OPvec3 a, OPmat4 b) {
	 OPvec3 result;
	 result.x = b[0].x * a.x + b[1].x * a.y + b[2].x * a.z + b[3][0] * 1.0f;
	 result.y = b[0].y * a.x + b[1].y * a.y + b[2].y * a.z + b[3][1] * 1.0f;
	 result.z = b[0].z * a.x + b[1].z * a.y + b[2].z * a.z + b[3][2] * 1.0f;
	 return result;
 }

inline OPvec4 OPmat4Transform(OPvec4 a, OPmat4 b) {
	OPvec4 result;
	result.x = b[0][0] * a.x + b[1][0] * a.y + b[2][0] * a.z + b[3][0] * a.w;
	result.y = b[0][1] * a.x + b[1][1] * a.y + b[2][1] * a.z + b[3][1] * a.w;
	result.z = b[0][2] * a.x + b[1][2] * a.y + b[2][2] * a.z + b[3][2] * a.w;
	result.w = b[0][3] * a.x + b[1][3] * a.y + b[2][3] * a.z + b[3][3] * a.w;
	return result;
}

 inline void OPmat4RotX(OPmat4* m, OPfloat x) {
	 OPmat4 tmp = OPmat4RotX(x);
	 OPmat4Mul(m, *m, tmp);
 }

 inline void OPmat4RotY(OPmat4* m, OPfloat x) {
	 OPmat4 tmp = OPmat4RotY(x);
	 OPmat4Mul(m, *m, tmp);
 }

 inline void OPmat4RotZ(OPmat4* m, OPfloat x) {
	 OPmat4 tmp = OPmat4RotZ(x);
	 OPmat4Mul(m, *m, tmp);
 }

 inline void OPmat4Translate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z){
	 m->cols[3].x += x;
	 m->cols[3].y += y;
	 m->cols[3].z += z;
 }

 inline void OPmat4Scl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z) {
	 OPmat4 tmp = OPmat4Scl(x, y, z);
	 OPmat4Mul(m, *m, tmp);
 }



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



inline OPfloat OPmat4GetCofactor(OPfloat m0, OPfloat m1, OPfloat m2,
	OPfloat m3, OPfloat m4, OPfloat m5,
	OPfloat m6, OPfloat m7, OPfloat m8)
{
	return m0 * (m4 * m8 - m5 * m7) -
		m1 * (m3 * m8 - m5 * m6) +
		m2 * (m3 * m7 - m4 * m6);
}


// Test methods

inline OPmat4 OPmat4RotationBetween(OPvec3 start, OPvec3 dest) {
	start = OPvec3Norm(start);
	dest = OPvec3Norm(dest);

	OPvec3 axis = OPvec3Cross(start, dest);
	f32 len = OPvec3Len(axis);
	if (len == 0) {
		return OPMAT4_IDENTITY;
	}

	f32 angle = OPasin(len);
	axis *= 1 / len;

	return OPmat4RotY(axis.y) * OPmat4RotZ(axis.z) * OPmat4RotX(axis.x);
}

inline OPmat4 OPmat4RotationNormal(OPvec3 normal) {
	OPvec3 up = OPVEC3_UP;
	normal = OPvec3Norm(normal);
	OPvec3 axis = OPvec3Norm(OPvec3Cross(up, normal));

	OPvec3 other = OPvec3Norm(OPvec3Cross(axis, normal));

	OPmat4 result = OPMAT4_IDENTITY;

	result[0][0] = axis.x;
	result[1][0] = axis.y;
	result[2][0] = axis.z;

	result[0][1] = normal.x;
	result[1][1] = normal.y;
	result[2][1] = normal.z;

	result[0][2] = other.x;
	result[1][2] = other.y;
	result[2][2] = other.z;

	return result;
}

inline OPmat4 OPmat4RotationNormal2(OPvec3 normal) {
	OPvec3 up = OPVEC3_UP;
	normal = OPvec3Norm(normal);
	OPvec3 axis = OPvec3Norm(OPvec3Cross(up, normal));

	f32 dot = OPvec3Dot(OPVEC3_UP, normal);
	f32 phi = OPacos(dot);

	OPmat4 result = OPMAT4_IDENTITY;

	f32 rcos = OPcos(phi);
	f32 rsin = OPsin(phi);
	f32 u = axis.x;
	f32 v = axis.y;
	f32 w = axis.z;
	result[0][0] = rcos + u*u*(1 - rcos);
	result[1][0] = w * rsin + v*u*(1 - rcos);
	result[2][0] = -v * rsin + w*u*(1 - rcos);
	result[0][1] = -w * rsin + u*v*(1 - rcos);
	result[1][1] = rcos + v*v*(1 - rcos);
	result[2][1] = u * rsin + w*v*(1 - rcos);
	result[0][2] = v * rsin + u*w*(1 - rcos);
	result[1][2] = -u * rsin + v*w*(1 - rcos);
	result[2][2] = rcos + w*w*(1 - rcos);

	return result;
}