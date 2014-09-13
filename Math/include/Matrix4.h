#pragma once

#ifndef OPMATH_MAT4
#define OPMATH_MAT4
#include "./Math/include/Vector2.h"
#include "./Math/include/Vector3.h"
#include "./Math/include/Vector4.h"
#include "./Math/include/Quaternion.h"
#include "./Core/include/DynamicMemory.h"
#include "./Core/include/MathHelpers.h"
#include "./Core/include/Log.h"

struct OPmat4;
inline OPvec4* OPmat4index(OPmat4* m, int idx);
inline void OPmat4mul(OPmat4* dst, OPmat4* m1, OPmat4* m2);
inline void OPmat4identity(OPmat4* m);
inline void OPmat4transpose(OPmat4* m);
inline void OPmat4buildRotX(OPmat4* m, OPfloat t);
inline void OPmat4buildRotY(OPmat4* m, OPfloat t);
inline void OPmat4buildRotZ(OPmat4* m, OPfloat t);
inline void OPmat4buildTranslate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);
inline void OPmat4buildScl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);
inline void OPmat4rotX(OPmat4* m, OPfloat x);
inline void OPmat4rotY(OPmat4* m, OPfloat x);
inline void OPmat4rotZ(OPmat4* m, OPfloat x);
inline void OPmat4translate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);
inline void OPmat4scl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);
inline void OPmat4ortho(OPmat4* m, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top, OPfloat zNear, OPfloat zFar );
inline void OPmat4look(OPmat4* m, OPvec3* campos, OPvec3* look, OPvec3* up);
inline void OPmat4Print(OPmat4 m);
inline void OPmat4perspective(OPmat4* m, OPfloat fovy, OPfloat aspect, OPfloat nearVal, OPfloat farVal);
inline void OPmat4transform(OPvec3* dst, OPvec3* a, OPmat4* b);

extern const OPmat4 OPmat4Zero;
extern const OPmat4 OPmat4Identity;

// Data Structure 4 * 4 = 16 floats
//		32 bit = 16 * 32 = 512 bits or 64 bytes
//		64 bit = 16 * 64 = 1024 bits or 128 bytes
struct OPmat4 {
	OPvec4 cols[4];
	OPmat4 operator=(OPmat4 vhs) { 
		OPmemcpy(this, (void*)&vhs, sizeof(OPmat4)); return *this;
	}
	inline OPmat4 operator*(OPmat4 vhs) { 
		OPmat4 temp = OPmat4Zero;
		OPmat4mul(&temp, this, &vhs); 
		return temp; 
	}
	inline OPmat4& operator*=(OPmat4 vhs) { 
		OPmat4mul(this, this, &vhs); 
		return *this; 
	}
	inline OPmat4& operator*=(OPvec2 vhs) {
		OPmat4scl(this, vhs.x, vhs.y, 1);
		return *this;
	}
	inline OPmat4& operator*=(OPvec3 vhs) {
		OPmat4scl(this, vhs.x, vhs.y, vhs.z);
		return *this;
	}
	inline OPmat4& operator*=(OPvec4 vhs) {
		OPmat4scl(this, vhs.x, vhs.y, vhs.z);
		return *this;
	}
	inline OPmat4& operator/=(OPvec2 vhs) {
		OPmat4scl(this, vhs.x, vhs.y, 1);
		return *this;
	}
	inline OPmat4& operator/=(OPvec3 vhs) { 
		OPmat4scl(this, vhs.x, vhs.y, vhs.z); 
		return *this; 
	}
	inline OPmat4& operator/=(OPvec4 vhs) { 
		OPmat4scl(this, vhs.x, vhs.y, vhs.z); 
		return *this; 
	}
	inline OPmat4& operator+=(OPvec2 vhs) {
		OPmat4translate(this, vhs.x, vhs.y, 0);
		return *this;
	}
	inline OPmat4& operator+=(OPvec2* vhs) {
		OPmat4translate(this, vhs->x, vhs->y, 0);
		return *this;
	}
	inline OPmat4& operator+=(OPvec3 vhs) {
		OPmat4translate(this, vhs.x, vhs.y, vhs.z);
		return *this;
	}
	inline OPmat4& operator+=(OPvec3* vhs) {
		OPmat4translate(this, vhs->x, vhs->y, vhs->z);
		return *this;
	}
	inline OPmat4& operator+=(OPvec4 vhs) {
		OPmat4translate(this, vhs.x, vhs.y, vhs.z);
		return *this;
	}
	inline OPmat4& operator+=(OPvec4* vhs) {
		OPmat4translate(this, vhs->x, vhs->y, vhs->z);
		return *this;
	}
	inline OPmat4& operator-=(OPvec2 vhs) {
		OPmat4translate(this, -vhs.x, -vhs.y, 0);
		return *this;
	}
	inline OPmat4& operator-=(OPvec2* vhs) {
		OPmat4translate(this, -vhs->x, -vhs->y, 0);
		return *this;
	}
	inline OPmat4& operator-=(OPvec3 vhs) {
		OPmat4translate(this, -vhs.x, -vhs.y, -vhs.z);
		return *this;
	}
	inline OPmat4& operator-=(OPvec3* vhs) {
		OPmat4translate(this, -vhs->x, -vhs->y, -vhs->z);
		return *this;
	}
	inline OPmat4& operator-=(OPvec4 vhs) {
		OPmat4translate(this, -vhs.x, -vhs.y, -vhs.z);
		return *this;
	}
	inline OPmat4& operator-=(OPvec4* vhs) {
		OPmat4translate(this, -vhs->x, -vhs->y, -vhs->z);
		return *this;
	}
};

inline OPvec3 operator*(OPvec3 lhs, OPmat4 rhs) { 
	OPvec3 temp = { 0, 0, 0 };
	OPmat4transform(&temp, &lhs, &rhs);
	return temp; 
}

inline OPvec3 operator*(OPmat4 lhs, OPvec3 rhs) { 
	OPvec3 temp = { 0, 0, 0 };
	OPmat4transform(&temp, &rhs, &lhs);
	return temp; 
}

inline OPvec4* OPmat4index(OPmat4* m, int idx){
	return &((OPvec4*)(m))[idx];
}

inline void OPmat4mul(OPmat4* dst, OPmat4* m1, OPmat4* m2) {
	OPmat4 result;
	OPfloat sum;
	i32 i, j, k = 0;
	OPfloat* cols1 = (OPfloat*)m1->cols;
	OPfloat* cols2 = (OPfloat*)m2->cols;
	OPfloat* colsr = (OPfloat*)result.cols;
	OPmemcpy(&result, m1, sizeof(OPmat4));
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			sum = 0;
			for(k = 0; k < 4; k++){
				sum += cols2[(k << 2) + j] * cols1[(i << 2) + k];
			}
			colsr[(i << 2) + j] = sum;
		}
	}
	OPmemcpy(dst, &result, sizeof(OPmat4));
}

inline void OPmat4identity(OPmat4* m) {
	OPbzero(m, sizeof(OPmat4));
	m->cols[0].x = 1;
	m->cols[1].y = 1;
	m->cols[2].z = 1;
	m->cols[3].w = 1;
}


 inline void OPmat4transpose(OPmat4* m){
	OPmat4* tmp;
	OPint i = 0, j = 0;
	OPmemcpy(&tmp, &m, sizeof(OPmat4*));
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			(*OPvec4index(OPmat4index(m, i),j)) = (*OPvec4index(OPmat4index(tmp, i),j));
		}
	}
}

inline void OPmat4buildRotX(OPmat4* m, OPfloat t) {
	OPfloat t1 = OPcos(t);
	OPfloat t2 = OPsin(t);
	OPmat4identity(m);
	m->cols[1].y = t1;
	m->cols[1].z = t2;
	m->cols[2].y = -t2;
	m->cols[2].z = t1;
}

inline OPmat4 OPmat4createRotX(OPfloat t) {
	OPmat4 temp;
	OPmat4buildRotX(&temp, t);
	return temp;
}

inline void OPmat4buildRotY(OPmat4* m, OPfloat t){
	OPfloat t1 = OPcos(t);
	OPfloat t2 = OPsin(t);
	OPmat4identity(m);
	m->cols[0].x = t1;
	m->cols[0].z = -t2;
	m->cols[2].x = t2;
	m->cols[2].z = t1;
}

inline OPmat4 OPmat4createRotY(OPfloat t) {
	OPmat4 temp;
	OPmat4buildRotY(&temp, t);
	return temp;
}

inline void OPmat4buildRotZ(OPmat4* m, OPfloat t) {
	OPfloat t1 = OPcos(t);
	OPfloat t2 = OPsin(t);
	OPmat4identity(m);
	m->cols[0].x = t1;
	m->cols[0].y = t2;
	m->cols[1].x = -t2;
	m->cols[1].y = t1;
}

inline OPmat4 OPmat4createRotZ(OPfloat t) {
	OPmat4 temp;
	OPmat4buildRotZ(&temp, t);
	return temp;
}

inline void OPmat4buildTranslate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z) {
	OPmat4identity(m);
	m->cols[3].x = x;
	m->cols[3].y = y;
	m->cols[3].z = z;
}

inline OPmat4 OPmat4createTranslate(OPfloat x, OPfloat y, OPfloat z) {
	OPmat4 temp;
	OPmat4buildTranslate(&temp, x, y, z);
	return temp;
}

inline void OPmat4buildScl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z) {
	OPmat4identity(m);
	m->cols[0].x = x;
	m->cols[1].y = y;
	m->cols[2].z = z;
}

inline OPmat4 OPmat4createScl(OPfloat x, OPfloat y, OPfloat z) {
	OPmat4 temp;
	OPmat4buildScl(&temp, x, y, z);
	return temp;
}



inline void OPmat4rotX(OPmat4* m, OPfloat x) {
	OPmat4 temp = OPmat4Zero;
	OPmat4buildRotX(&temp, x);
	OPmat4mul(m, m, &temp);
}

inline void OPmat4rotZ(OPmat4* m, OPfloat x) {
	OPmat4 temp = OPmat4Zero;
	OPmat4buildRotZ(&temp, x);
	OPmat4mul(m, m, &temp);
}

inline void OPmat4translate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z) {
	OPmat4 temp = OPmat4Zero;
	OPmat4buildTranslate(&temp, x, y, z);
	OPmat4mul(m, m, &temp);
}

inline void OPmat4scl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z) {
	OPmat4 temp = OPmat4Zero;
	OPmat4buildScl(&temp, x, y, z);
	OPmat4mul(m, m, &temp);
}

inline void OPmat4rotY(OPmat4* m, OPfloat x) {
	OPmat4 temp = OPmat4Zero;
	OPmat4buildRotY(&temp, x);
	OPmat4mul(m, m, &temp);
}

inline void OPmat4ortho(OPmat4* m, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top, OPfloat zNear, OPfloat zFar ){
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
	OPvec3  f, u, s;
	OPvec3 dist;
	OPvec3sub(&dist, at, eye);
	OPvec3norm(&f, &dist);
	OPvec3norm(&u, up);
	OPvec3cross(&s, &f, &u);
	OPvec3norm(&s, &s);
	OPvec3cross(&u, &s, &f);

	OPfloat sDot, uDot, fDot;
	OPvec3dot(&sDot, &s, eye);
	OPvec3dot(&uDot, &u, eye);
	OPvec3dot(&fDot, &f, eye);

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

inline void OPmat4perspective(OPmat4* m, OPfloat fovy, OPfloat aspect, OPfloat nearVal, OPfloat farVal) {
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

inline void OPmat4transform(OPvec3* dst, OPvec3* a, OPmat4* b) {
	dst->x = b->cols[0].x * a->x + b->cols[1].x * a->y + b->cols[2].x * a->z;
	dst->y = b->cols[0].y * a->x + b->cols[1].y * a->y + b->cols[2].y * a->z;
	dst->z = b->cols[0].z * a->x + b->cols[1].z * a->y + b->cols[2].z * a->z;
}

inline void OPmat4transformVec4(OPvec4* dst, OPvec4* a, OPmat4* b) {
	dst->x = b->cols[0].x * a->x + b->cols[1].x * a->y + b->cols[2].x * a->z + b->cols[3].x * a->w;
	dst->y = b->cols[0].y * a->x + b->cols[1].y * a->y + b->cols[2].y * a->z + b->cols[3].y * a->w;
	dst->z = b->cols[0].z * a->x + b->cols[1].z * a->y + b->cols[2].z * a->z + b->cols[3].z * a->w;
	dst->w = b->cols[0].w * a->x + b->cols[1].w * a->y + b->cols[2].w * a->z + b->cols[3].w * a->w;
}

inline void OPmat4buildQuat(OPmat4* dst, OPquat* qtr){
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
	OPmat4 temp = OPmat4Zero;
	OPmat4buildQuat(&temp, qtr);
	OPmat4mul(m, m, &temp);
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
		OPmat4identity(dst);
		return;
	}

	OPmat4scl(dst, 1 / det, 1 / det, 1 / det);
}

#endif
