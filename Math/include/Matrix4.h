#pragma once

#ifndef OPMATH_MAT4
#define OPMATH_MAT4
#include "./Math/include/Vector3.h"
#include "./Math/include/Vector4.h"
#include "./Core/include/DynamicMemory.h"
#include "./Core/include/MathHelpers.h"


#define OPmat4mul(dst, m1, m2) {\
	OPmat4 result;\
	OPfloat sum;\
	i32 i, j, k = 0;\
	OPmemcpy(&result, &m1, sizeof(OPmat4));\
	for(i = 0; i < 4; i++){\
		for(j = 0; j < 4; j++){\
			sum = 0;\
			for(k = 0; k < 4; k++){\
				sum += m2->cols[k][j] * m1->cols[i][k];\
			}\
			result.cols[i][j] = sum;\
		}\
	}\
	OPmemcpy(&dest, &result, sizeof(OPmat4));\
}\

#define OPmat4identity(m) {\
	OPbzero(&m, sizeof(OPmat4));\
	m.cols[0].x = 1;\
	m.cols[1].y = 1;\
	m.cols[2].z = 1;\
	m.cols[3].w = 1;\
}\


#define OPmat4rotX(m, x) {\
	OPfloat t1 = OPcos(x);\
	OPfloat t2 = OPsin(x);\
	m.cols[1].y *= t1;\
	m.cols[1].z *= t2;\
	m.cols[2].y *= -t2;\
	m.cols[2].z *= t1;\
}\

#define OPmat4rotY(m, x) {\
	OPfloat t1 = OPcos(x);\
	OPfloat t2 = OPsin(x);\
	m.cols[0].x *= t1;\
	m.cols[0].z *= -t2;\
	m.cols[2].x *= t2;\
	m.cols[2].z *= t1;\
}\

#define OPmat4rotZ(m, x) {\
	OPfloat t1 = OPcos(x);\
	OPfloat t2 = OPsin(x);\
	m.cols[0].x *= t1;\
	m.cols[0].y *= t2;\
	m.cols[1].x *= -t2;\
	m.cols[1].y *= t1;\
}\

#define OPmat4translate(m, x, y, z) {\
	m.cols[3].x += x;\
	m.cols[3].y += y;\
	m.cols[3].z += z;\
}\

#define OPmat4scl(m, x, y, z) {\
	OPmat4identity(m);\
	m.cols[0].x = x;\
	m.cols[1].y = y;\
	m.cols[2].z = z;\
}\

#define OPmat4transpose(m){\
	OPmat4 tmp;\
	OPint i = 0, j = 0;\
	OPmemcpy(&tmp, &m, sizeof(OPmat4));\
	for(i = 0; i < 4; i++){\
		for(j = 0; j < 4; j++){\
			m.cols[i][j] = tmp.cols[i][j];\
		}\
	}\
}\

#define OPmat4buildRotX(m, t) {\
	OPfloat t1 = OPcos(t);\
	OPfloat t2 = OPsin(t);\
	OPmat4identity(m);\
	m.cols[1].y = t1;\
	m.cols[1].z = t2;\
	m.cols[2].y = -t2;\
	m.cols[2].z = t1;\
}\

#define OPmat4buildRotY(m, t){\
	OPfloat t1 = OPcos(t);\
	OPfloat t2 = OPsin(t);\
	OPmat4identity(m);\
	m.cols[0].x = t1;\
	m.cols[0].z = -t2;\
	m.cols[2].x = t2;\
	m.cols[2].z = t1;\
}\

#define OPmat4buildRotZ(m, t) {\
	OPfloat t1 = OPcos(t);\
	OPfloat t2 = OPsin(t);\
	OPmat4identity(m);\
	m.cols[0].x = t1;\
	m.cols[0].y = t2;\
	m.cols[1].x = -t2;\
	m.cols[1].y = t1;\
}\

#define OPmat4buildTranslate(m, x, y, z) {\
	OPmat4identity(m);\
	m.cols[3].x = x;\
	m.cols[3].y = y;\
	m.cols[3].z = z;\
}\

#define OPmat4buildScl(m, x, y, z) {\
	OPmat4identity(m);\
	m.cols[0].x = x;\
	m.cols[1].y = y;\
	m.cols[2].z = z;\
}\

#define OPmat4perspective(m, fovy, aspect, near, far) {\
	OPfloat fovyD = (fovy/2.0f) * (OPpi / 180.0f);\
	OPfloat range = tan(fovyD) * near;\
	OPfloat left = -range * aspect;\
	OPfloat right = range * aspect;\
	OPfloat bottom = -range;\
	OPfloat top = range;\
	OPbzero(&m, sizeof(OPmat4));\
	m.cols[0].x = ( 2 * near ) / ( right - left );\
	m.cols[1].y = ( 2 * near ) / ( top - bottom );\
	m.cols[2].z = - ( far + near ) / ( far - near);\
	m.cols[2].w = - 1;\
	m.cols[3].z = - (2 * far * near) / ( far - near );\
	m.cols[3].w = 1;\
}\

#define OPmat4ortho(m, left, right, bottom, top, zNear, zFar ){\
		OPbzero(&m, sizeof(OPmat4));\
		m.cols[0].x = 2.0/(right - left);\
		m.cols[1].y = -2.0/(top - bottom);\
		m.cols[2].z = 2.0/(zNear - zFar);\
		m.cols[3].w = 1.0;\
		m.cols[3].x = -(right + left)/(right - left);\
		m.cols[3].y = -(top + bottom)/(top - bottom);\
		m.cols[3].z = -(zFar + zNear)/(zFar - zNear);\
}\

#define OPmat4look(m, campos, look, up) {\
	OPvec3 f;\
	OPvec3 s;\
	OPvec3 u;\
	OPint i = 0;\
	OPvec3sub(f, look, campos);\
	OPvec3norm(f, f);\
	OPvec3norm(up, up);\
	OPvec3cross(s, f, up);\
	OPvec3norm(s, s);\
	OPvec3cross(u, s, f);\
	OPmat4identity(m);\
	for(i = 0; i < 3; i++){\
		m.cols[i].x = ((OPfloat*)&s)[i];\
	}\
	for(i = 0; i < 3; i++){\
		m.cols[i].y = ((OPfloat*)&u)[i];\
	}\
	for(i = 0; i < 3; i++){\
		m.cols[i].z = -((OPfloat*)&f)[i];\
	}\
	m.cols[3].x += campos.x;\
	m.cols[3].y += campos.y;\
	m.cols[3].z += campos.z;\
} \



// Data Structure 4 * 4 = 16 floats
//		32 bit = 16 * 32 = 512 bits or 64 bytes
//		64 bit = 16 * 64 = 1024 bits or 128 bytes
typedef struct {
	OPvec4 cols[4];
} OPmat4;

#endif