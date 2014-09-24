#include "./Math/include/BoundingSphere3D.h"

OPint OPboundingSphere3DCollision(OPboundingSphere3D* obj1, OPboundingSphere3D* obj2)
{
	OPvec3 relPos = OPvec3Create(
		obj1->pos.x - obj2->pos.x,
		obj1->pos.y - obj2->pos.y,
		obj1->pos.z - obj2->pos.z
		);
	OPfloat dist = relPos.x * relPos.x + relPos.y * relPos.y + relPos.z * relPos.z;
	OPfloat minDist = obj1->radius + obj2->radius;
	return dist <= minDist * minDist;
}