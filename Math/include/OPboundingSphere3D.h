#pragma once

struct OPboundingSphere3D;
typedef struct OPboundingSphere3D OPboundingSphere3D;

#include "./Math/include/OPvec3.h"

struct OPboundingSphere3D {
	OPvec3 pos;
	OPfloat radius;

	inline bool Collision(OPboundingSphere3D* obj2) {
		OPvec3 relPos = pos - obj2->pos;
		OPfloat dist = relPos.x * relPos.x + relPos.y * relPos.y + relPos.z * relPos.z;
		OPfloat minDist = radius + obj2->radius;
		return dist <= (minDist * minDist);
	}
};