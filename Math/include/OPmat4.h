#pragma once

#ifndef OPMATH_MAT4
#define OPMATH_MAT4
#include "./Math/include/OPvec2.h"
#include "./Math/include/OPvec3.h"
#include "./Math/include/OPvec4.h"
#include "./Math/include/OPquat.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPmath.h"
#include "./Core/include/OPlog.h"

struct OPmat4;
inline OPvec4* OPmat4Index(OPmat4* m, int idx);
inline void OPmat4Mul(OPmat4* dst, OPmat4* m1, OPmat4* m2);
inline void OPmat4Identity(OPmat4* m);
inline void OPmat4Transpose(OPmat4* m);
inline void OPmat4BuildRotX(OPmat4* m, OPfloat t);
inline void OPmat4BuildRotY(OPmat4* m, OPfloat t);
inline void OPmat4BuildRotZ(OPmat4* m, OPfloat t);
inline void OPmat4BuildTranslate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);
inline void OPmat4BuildTranslate(OPmat4* m, OPvec3* v);
inline void OPmat4BuildScl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);
inline void OPmat4RotX(OPmat4* m, OPfloat x);
inline void OPmat4RotY(OPmat4* m, OPfloat x);
inline void OPmat4RotZ(OPmat4* m, OPfloat x);
inline void OPmat4Translate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);
inline void OPmat4Translate(OPmat4* m, OPvec3* v);
inline void OPmat4Scl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);
inline void OPmat4Ortho(OPmat4* m, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top, OPfloat zNear, OPfloat zFar );
inline void OPmat4look(OPmat4* m, OPvec3* campos, OPvec3* look, OPvec3* up);
inline void OPmat4Print(OPmat4 m);
inline void OPmat4Perspective(OPmat4* m, OPfloat fovy, OPfloat aspect, OPfloat nearVal, OPfloat farVal);
inline void OPmat4Transform(OPvec3* dst, OPvec3* a, OPmat4* b);


extern const OPmat4 OPMAT4ZERO;
extern const OPmat4 OPMAT4IDENTITY;

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
	inline OPmat4& operator*=(OPmat4 vhs) { 
		OPmat4Mul(this, this, &vhs); 
		return *this; 
	}
	inline OPmat4& operator*=(OPvec2 vhs) {
		OPmat4Scl(this, vhs.x, vhs.y, 1);
		return *this;
	}
	inline OPmat4& operator*=(OPvec3 vhs) {
		OPmat4Scl(this, vhs.x, vhs.y, vhs.z);
		return *this;
	}
	inline OPmat4& operator*=(OPvec4 vhs) {
		OPmat4Scl(this, vhs.x, vhs.y, vhs.z);
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
};

inline OPvec3 operator*(OPvec3 lhs, OPmat4 rhs) { 
	OPvec3 temp = { 0, 0, 0 };
	OPmat4Transform(&temp, &lhs, &rhs);
	return temp; 
}

inline OPvec3 operator*(OPmat4 lhs, OPvec3 rhs) { 
	OPvec3 temp = { 0, 0, 0 };
	OPmat4Transform(&temp, &rhs, &lhs);
	return temp; 
}

inline OPmat4 operator*(OPmat4 lhs, OPmat4 rhs) {
	OPmat4 temp;
	OPmat4Mul(&temp, &lhs, &rhs);
	return temp;
}


inline OPmat4 OPmat4create(
	f32 _00, f32 _10, f32 _20, f32 _30,
	f32 _01, f32 _11, f32 _21, f32 _31,
	f32 _02, f32 _12, f32 _22, f32 _32,
	f32 _03, f32 _13, f32 _23, f32 _33) {

	OPmat4 temp = {
		_00, _10, _20, _30,
		_01, _11, _21, _31,
		_02, _12, _22, _32,
		_03, _13, _23, _33
	};

	return temp;
}

inline OPmat4 OPmat4createFromVec4(OPvec4 row0, OPvec4 row1, OPvec4 row2, OPvec4 row3) {
	return OPmat4create(
		row0.x, row0.y, row0.z, row0.w,
		row1.x, row1.y, row1.z, row1.w,
		row2.x, row2.y, row2.z, row2.w,
		row3.x, row3.y, row3.z, row3.w
		);
}

inline OPvec4* OPmat4Index(OPmat4* m, int idx){
	return &((OPvec4*)(m))[idx];
}

inline void OPmat4Mul(OPmat4* dst, OPmat4* mat1, OPmat4* mat2)
{
	OPmat4 result;
	OPmat4 m1 = *mat1;
	OPmat4 m2 = *mat2;
	result[0][0] = m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2] + m1[3][0] * m2[0][3]; 
	result[1][0] = m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2] + m1[3][0] * m2[1][3]; 
	result[2][0] = m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2] + m1[3][0] * m2[2][3]; 
	result[3][0] = m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1] + m1[2][0] * m2[3][2] + m1[3][0] * m2[3][3]; 
	result[0][1] = m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2] + m1[3][1] * m2[0][3]; 
	result[1][1] = m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2] + m1[3][1] * m2[1][3]; 
	result[2][1] = m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2] + m1[3][1] * m2[2][3]; 
	result[3][1] = m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1] + m1[2][1] * m2[3][2] + m1[3][1] * m2[3][3]; 
	result[0][2] = m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2] + m1[3][2] * m2[0][3]; 
	result[1][2] = m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2] + m1[3][2] * m2[1][3]; 
	result[2][2] = m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2] + m1[3][2] * m2[2][3]; 
	result[3][2] = m1[0][2] * m2[3][0] + m1[1][2] * m2[3][1] + m1[2][2] * m2[3][2] + m1[3][2] * m2[3][3]; 
	result[0][3] = m1[0][3] * m2[0][0] + m1[1][3] * m2[0][1] + m1[2][3] * m2[0][2] + m1[3][3] * m2[0][3]; 
	result[1][3] = m1[0][3] * m2[1][0] + m1[1][3] * m2[1][1] + m1[2][3] * m2[1][2] + m1[3][3] * m2[1][3]; 
	result[2][3] = m1[0][3] * m2[2][0] + m1[1][3] * m2[2][1] + m1[2][3] * m2[2][2] + m1[3][3] * m2[2][3]; 
	result[3][3] = m1[0][3] * m2[3][0] + m1[1][3] * m2[3][1] + m1[2][3] * m2[3][2] + m1[3][3] * m2[3][3]; 
	OPmemcpy(dst, &result, sizeof(OPmat4));
};

inline void OPmat4Identity(OPmat4* m) {
	OPbzero(m, sizeof(OPmat4));
	m->cols[0].x = 1;
	m->cols[1].y = 1;
	m->cols[2].z = 1;
	m->cols[3].w = 1;
}


 inline void OPmat4Transpose(OPmat4* m){
	OPmat4* tmp;
	OPint i = 0, j = 0;
	OPmemcpy(&tmp, &m, sizeof(OPmat4*));
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			(*OPmat4Index(m, i))[j] = (*OPmat4Index(tmp, i))[j];
		}
	}
}

inline void OPmat4BuildRotX(OPmat4* m, OPfloat t) {
	OPfloat t1 = OPcos(t);
	OPfloat t2 = OPsin(t);
	OPmat4Identity(m);
	m->cols[1].y = t1;
	m->cols[1].z = t2;
	m->cols[2].y = -t2;
	m->cols[2].z = t1;
}

inline OPmat4 OPmat4createRotX(OPfloat t) {
	OPmat4 temp;
	OPmat4BuildRotX(&temp, t);
	return temp;
}

inline void OPmat4BuildRotY(OPmat4* m, OPfloat t){
	OPfloat t1 = OPcos(t);
	OPfloat t2 = OPsin(t);
	OPmat4Identity(m);
	m->cols[0].x = t1;
	m->cols[0].z = -t2;
	m->cols[2].x = t2;
	m->cols[2].z = t1;
}

inline OPmat4 OPmat4createRotY(OPfloat t) {
	OPmat4 temp;
	OPmat4BuildRotY(&temp, t);
	return temp;
}

inline void OPmat4BuildRotZ(OPmat4* m, OPfloat t) {
	OPfloat t1 = OPcos(t);
	OPfloat t2 = OPsin(t);
	OPmat4Identity(m);
	m->cols[0].x = t1;
	m->cols[0].y = t2;
	m->cols[1].x = -t2;
	m->cols[1].y = t1;
}

inline OPmat4 OPmat4createRotZ(OPfloat t) {
	OPmat4 temp;
	OPmat4BuildRotZ(&temp, t);
	return temp;
}

inline void OPmat4BuildTranslate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z) {
	OPmat4Identity(m);
	m->cols[3].x = x;
	m->cols[3].y = y;
	m->cols[3].z = z;
}

inline void OPmat4BuildTranslate(OPmat4* m, OPvec3* v) {
	OPmat4Identity(m);
	m->cols[3].x = v->x;
	m->cols[3].y = v->y;
	m->cols[3].z = v->z;
}

inline OPmat4 OPmat4BuildTranslate(OPfloat x, OPfloat y, OPfloat z) {
	OPmat4 temp;
	OPmat4BuildTranslate(&temp, x, y, z);
	return temp;
}

inline OPmat4 OPmat4BuildTranslate(OPvec3* v) {
	OPmat4 temp;
	OPmat4BuildTranslate(&temp, v->x, v->y, v->z);
	return temp;
}

inline void OPmat4BuildScl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z) {
	OPmat4Identity(m);
	m->cols[0].x = x;
	m->cols[1].y = y;
	m->cols[2].z = z;
}

inline OPmat4 OPmat4createScl(OPfloat x, OPfloat y, OPfloat z) {
	OPmat4 temp;
	OPmat4BuildScl(&temp, x, y, z);
	return temp;
}



inline void OPmat4RotX(OPmat4* m, OPfloat x) {
	OPmat4 temp = OPMAT4ZERO;
	OPmat4BuildRotX(&temp, x);
	OPmat4Mul(m, m, &temp);
}

inline void OPmat4RotZ(OPmat4* m, OPfloat x) {
	OPmat4 temp = OPMAT4ZERO;
	OPmat4BuildRotZ(&temp, x);
	OPmat4Mul(m, m, &temp);
}

inline void OPmat4Translate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z) {
	(*m)[3][0] += x;
	(*m)[3][1] += y;
	(*m)[3][2] += z;
}

inline void OPmat4Translate(OPmat4* m, OPvec3* v) {
	(*m)[3][0] += v->x;
	(*m)[3][1] += v->y;
	(*m)[3][2] += v->z;
}

inline void OPmat4Scl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z) {
	OPmat4 temp = OPMAT4ZERO;
	OPmat4BuildScl(&temp, x, y, z);
	OPmat4Mul(m, m, &temp);
}

inline void OPmat4RotY(OPmat4* m, OPfloat x) {
	OPmat4 temp = OPMAT4ZERO;
	OPmat4BuildRotY(&temp, x);
	OPmat4Mul(m, m, &temp);
}

inline void OPmat4Ortho(OPmat4* m, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top, OPfloat zNear, OPfloat zFar ){
	OPbzero(m, sizeof(OPmat4));
	m->cols[0].x = 2.0f/(right - left);
	m->cols[1].y = -2.0f/(top - bottom);
	m->cols[2].z = 2.0f/(zNear - zFar);
	m->cols[3].w = 1.0f;
	m->cols[3].x = -(right + left)/(right - left);
	m->cols[3].y = -(top + bottom)/(top - bottom);
	m->cols[3].z = -(zFar + zNear)/(zFar - zNear);
}

inline void OPmat4look(OPmat4* m, OPvec3* eye, OPvec3* at, OPvec3* up) {
	OPvec3 dist = *at - *eye;
	OPvec3 f = OPvec3Norm(dist);
	OPvec3 u = OPvec3Norm(*up);
	OPvec3 s = OPvec3Cross(f, u);
	s = OPvec3Norm(s);
	u = OPvec3Cross(s, f);

	OPfloat sDot = OPvec3Dot(s, *eye);
	OPfloat uDot = OPvec3Dot(u, *eye);
	OPfloat fDot = OPvec3Dot(f, *eye);

	m->cols[0].x = s.x;
	m->cols[1].x = s.y;
	m->cols[2].x = s.z;
	m->cols[0].y = u.x;
	m->cols[1].y = u.y;
	m->cols[2].y = u.z;
	m->cols[0].z = -f.x;
	m->cols[1].z = -f.y;
	m->cols[2].z = -f.z;
	m->cols[3].x = -sDot;
	m->cols[3].y = -uDot;
	m->cols[3].z = fDot;
}

inline void OPmat4Print(OPmat4 m){
	OPlog("[ %.6f  %.6f  %.6f  %.6f ]", m.cols[0].x,m.cols[1].x,m.cols[2].x,m.cols[3].x);
	OPlog("[ %.6f  %.6f  %.6f  %.6f ]", m.cols[0].y,m.cols[1].y,m.cols[2].y,m.cols[3].y);
	OPlog("[ %.6f  %.6f  %.6f  %.6f ]", m.cols[0].z,m.cols[1].z,m.cols[2].z,m.cols[3].z);
	OPlog("[ %.6f  %.6f  %.6f  %.6f ]", m.cols[0].w,m.cols[1].w,m.cols[2].w,m.cols[3].w);
}

inline void OPmat4Perspective(OPmat4* m, OPfloat fovy, OPfloat aspect, OPfloat nearVal, OPfloat farVal) {
	OPfloat top = OPtan(fovy * OPpi / 360.0f) * nearVal;
	OPfloat right = top * aspect;
	OPfloat range = farVal - nearVal;
	OPbzero(m, sizeof(OPmat4));
	OPvec4 c0 = {nearVal/right, 0, 0, 0};
	OPvec4 c1 = {0, nearVal/top, 0, 0};
	OPvec4 c2 = {0, 0, -(farVal+nearVal)/range, -1};
	OPvec4 c3 = {0, 0, -2.0f*farVal*nearVal/range, 1};
	m->cols[0] = c0;
	m->cols[1] = c1;
	m->cols[2] = c2;
	m->cols[3] = c3;
}

inline void OPmat4Transform(OPvec3* dst, OPvec3* a, OPmat4* b) {
	dst->x = b->cols[0].x * a->x + b->cols[1].x * a->y + b->cols[2].x * a->z;
	dst->y = b->cols[0].y * a->x + b->cols[1].y * a->y + b->cols[2].y * a->z;
	dst->z = b->cols[0].z * a->x + b->cols[1].z * a->y + b->cols[2].z * a->z;
}

inline void OPmat4TransformVec4(OPvec4* dst, OPvec4* a, OPmat4* b) {
	dst->x = b->cols[0].x * a->x + b->cols[1].x * a->y + b->cols[2].x * a->z + b->cols[3].x * a->w;
	dst->y = b->cols[0].y * a->x + b->cols[1].y * a->y + b->cols[2].y * a->z + b->cols[3].y * a->w;
	dst->z = b->cols[0].z * a->x + b->cols[1].z * a->y + b->cols[2].z * a->z + b->cols[3].z * a->w;
	dst->w = b->cols[0].w * a->x + b->cols[1].w * a->y + b->cols[2].w * a->z + b->cols[3].w * a->w;
}

inline void OPmat4BuildQuat(OPmat4* dst, OPquat* qtr){
	f32 x = qtr->x;		f32 y = qtr->y;		f32 z = qtr->z;		f32 w = qtr->w;
	f32 x2 = x + x;		f32 y2 = y + y;		f32 z2 = z + z;
	f32 xx = x * x2;	f32 xy = x * y2;	f32 xz = x * z2;
	f32 yy = y * y2;	f32 yz = y * z2;	f32 zz = z * z2;
	f32 wx = w * x2;	f32 wy = w * y2;	f32 wz = w * z2;

	dst->cols[0].x = 1 - (yy + zz);
	dst->cols[0].y = xy - wz;
	dst->cols[0].z = xz + wy;

	dst->cols[1].x = xy + wz;
	dst->cols[1].y = 1 - (xx + zz);
	dst->cols[1].z = yz - wx;
	
	dst->cols[2].x = xz - wy;
	dst->cols[2].y = yz + wx;
	dst->cols[2].z = 1 - (xx + yy);

	dst->cols[3].x = 0;
	dst->cols[3].y = 0;
	dst->cols[3].z = 0;
	
	dst->cols[0].w = 0;
	dst->cols[1].w = 0;
	dst->cols[2].w = 0;
	dst->cols[3].w = 1;
}

inline void OPmat4quat(OPmat4* m, OPquat* qtr) {
	OPmat4 temp = OPMAT4ZERO;
	OPmat4BuildQuat(&temp, qtr);
	OPmat4Mul(m, m, &temp);
}

inline void OPmat4Inverse(OPmat4* dst, OPmat4* src) {

	// based on http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm

	f32 n11 = src->cols[0].x, n12 = src->cols[0].y, n13 = src->cols[0].z, n14 = src->cols[0].w;
	f32 n21 = src->cols[1].x, n22 = src->cols[1].y, n23 = src->cols[1].z, n24 = src->cols[1].w;
	f32 n31 = src->cols[2].x, n32 = src->cols[2].y, n33 = src->cols[2].z, n34 = src->cols[2].w;
	f32 n41 = src->cols[3].x, n42 = src->cols[3].y, n43 = src->cols[3].z, n44 = src->cols[3].w;

	dst->cols[0].x = n23*n34*n42 - n24*n33*n42 + n24*n32*n43 - n22*n34*n43 - n23*n32*n44 + n22*n33*n44;
	dst->cols[0].y = n14*n33*n42 - n13*n34*n42 - n14*n32*n43 + n12*n34*n43 + n13*n32*n44 - n12*n33*n44;
	dst->cols[0].z = n13*n24*n42 - n14*n23*n42 + n14*n22*n43 - n12*n24*n43 - n13*n22*n44 + n12*n23*n44;
	dst->cols[0].w = n14*n23*n32 - n13*n24*n32 - n14*n22*n33 + n12*n24*n33 + n13*n22*n34 - n12*n23*n34;
	dst->cols[1].x = n24*n33*n41 - n23*n34*n41 - n24*n31*n43 + n21*n34*n43 + n23*n31*n44 - n21*n33*n44;
	dst->cols[1].y = n13*n34*n41 - n14*n33*n41 + n14*n31*n43 - n11*n34*n43 - n13*n31*n44 + n11*n33*n44;
	dst->cols[1].z = n14*n23*n41 - n13*n24*n41 - n14*n21*n43 + n11*n24*n43 + n13*n21*n44 - n11*n23*n44;
	dst->cols[1].w = n13*n24*n31 - n14*n23*n31 + n14*n21*n33 - n11*n24*n33 - n13*n21*n34 + n11*n23*n34;
	dst->cols[2].x = n22*n34*n41 - n24*n32*n41 + n24*n31*n42 - n21*n34*n42 - n22*n31*n44 + n21*n32*n44;
	dst->cols[2].y = n14*n32*n41 - n12*n34*n41 - n14*n31*n42 + n11*n34*n42 + n12*n31*n44 - n11*n32*n44;
	dst->cols[2].z = n12*n24*n41 - n14*n22*n41 + n14*n21*n42 - n11*n24*n42 - n12*n21*n44 + n11*n22*n44;
	dst->cols[2].w = n14*n22*n31 - n12*n24*n31 - n14*n21*n32 + n11*n24*n32 + n12*n21*n34 - n11*n22*n34;
	dst->cols[3].x = n23*n32*n41 - n22*n33*n41 - n23*n31*n42 + n21*n33*n42 + n22*n31*n43 - n21*n32*n43;
	dst->cols[3].y = n12*n33*n41 - n13*n32*n41 + n13*n31*n42 - n11*n33*n42 - n12*n31*n43 + n11*n32*n43;
	dst->cols[3].z = n13*n22*n41 - n12*n23*n41 - n13*n21*n42 + n11*n23*n42 + n12*n21*n43 - n11*n22*n43;
	dst->cols[3].w = n12*n23*n31 - n13*n22*n31 + n13*n21*n32 - n11*n23*n32 - n12*n21*n33 + n11*n22*n33;

	f32 det = n11 * dst->cols[0].x + n21 * dst->cols[0].y + n31 * dst->cols[0].z + n41 * dst->cols[0].w;

	if (det == 0) {
		// can't invert matrix, determinant is 0";
		OPmat4Identity(dst);
		return;
	}

	OPmat4Scl(dst, 1 / det, 1 / det, 1 / det);
}

inline void OPmat4Log(const OPchar* msg, OPmat4* mat) {
	OPmat4 m = *mat;
	OPlog("%s:\n\t%f,\t%f,\t%f,\t%f\n\t%f,\t%f,\t%f,\t%f\n\t%f,\t%f,\t%f,\t%f\n\t%f,\t%f,\t%f,\t%f",
		msg, 
		m[0][0], m[1][0], m[2][0], m[3][0], 
		m[0][1], m[1][1], m[2][1], m[3][1], 
		m[0][2], m[1][2], m[2][2], m[3][2], 
		m[0][3], m[1][3], m[2][3], m[3][3]);
}

#endif
