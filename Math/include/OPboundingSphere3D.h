#ifndef OP_MATH_BOUNDINGSPHERE3D
#define OP_MATH_BOUNDINGSPHERE3D

#include "./Core/include/OPtypes.h"
#include "./Math/include/OPvec3.h"

typedef struct {
	OPvec3 pos;
	OPfloat radius;
} OPboundingSphere3D;

static inline OPint OPboundingSphere3DCollision(OPboundingSphere3D* obj1, OPboundingSphere3D* obj2){
	OPvec3 relPos = obj1->pos - obj2->pos;
	OPfloat dist = relPos.x * relPos.x + relPos.y * relPos.y + relPos.z * relPos.z;
	OPfloat minDist = obj1->radius + obj2->radius;
	// OPvec3Log("obj1", obj1->pos);
	// OPvec3Log("obj2", obj2->pos);
	// OPvec3Log("rel", relPos);
	// OPlog("dist %f, min: %f", dist, minDist);
	return dist <= (minDist * minDist);
}

#endif