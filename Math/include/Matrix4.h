#pragma once

#ifndef OPMATH_MAT4
#define OPMATH_MAT4
#include "./Math/include/Vector3.h"
#include "./Math/include/Vector4.h"
#include "./Core/include/DynamicMemory.h"
#include "./Core/include/MathHelpers.h"
#include "./Core/include/Log.h"

// Data Structure 4 * 4 = 16 floats
//		32 bit = 16 * 32 = 512 bits or 64 bytes
//		64 bit = 16 * 64 = 1024 bits or 128 bytes
typedef struct {
	OPvec4 cols[4];
} OPmat4;

inline void OPmat4mul(OPmat4* dst, OPmat4* m1, OPmat4* m2);

inline void OPmat4identity(OPmat4* m);

inline void OPmat4rotX(OPmat4* m, OPfloat x);

inline void OPmat4rotY(OPmat4* m, OPfloat x);

inline void OPmat4rotZ(OPmat4* m, OPfloat x);

inline void OPmat4translate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);

inline void OPmat4scl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);

inline void OPmat4transpose(OPmat4* m);

inline void OPmat4buildRotX(OPmat4* m, OPfloat t);

inline void OPmat4buildRotY(OPmat4* m, OPfloat t);

inline void OPmat4buildRotZ(OPmat4* m, OPfloat t) ;

inline void OPmat4buildTranslate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);

inline void OPmat4buildScl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);

inline void OPmat4ortho(OPmat4* m, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top, OPfloat zNear, OPfloat zFar );

inline void OPmat4look(OPmat4* m, OPvec3* campos, OPvec3* look, OPvec3* up);

inline void OPmat4Print(OPmat4 m);

inline void OPmat4perspective(OPmat4* m, OPfloat fovy, OPfloat aspect, OPfloat near, OPfloat far);

#endif