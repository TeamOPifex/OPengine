#ifndef OP_MATH_BOUNDINGSPHERE3D
#define OP_MATH_BOUNDINGSPHERE3D

#include "./Core/include/Types.h"
#include "./Math/include/Vector3.h"

typedef struct {
	OPvec3 pos;
	OPfloat radius;
} OPboundingSphere3D;

OPint OPboundingSphere3DCollision(OPboundingSphere3D* obj1, OPboundingSphere3D* obj2);

#endif