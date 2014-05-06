#ifndef OP_MATH_BOUNDINGBOX3D
#define OP_MATH_BOUNDINGBOX3D

#include "Vector3.h"

typedef struct {
	OPvec3 min;
	OPvec3 max;
} OPboundingBox3D;

inline OPboundingBox3D OPboundingBox3DCreate(OPvec3 min, OPvec3 max) {
	OPboundingBox3D tmp = {
		min,
		max
	};
	return tmp;
}

inline void OPboundingBox3DAddVec3(OPboundingBox3D* bb, OPvec3* point) {
	bb->min += (*point);
	bb->max += (*point);
}

inline OPint OPboundingBox3DContainsVec3(OPboundingBox3D* bb, OPvec3* point) {
	return
		point->x >= bb->min.x && point->x <= bb->max.x &&
		point->y >= bb->min.y && point->y <= bb->max.y &&
		point->z >= bb->min.z && point->z <= bb->max.z;
}

inline OPint OPboundingBox3DContainsBoundingBox3D(OPboundingBox3D* bb, OPboundingBox3D* bb2) {
	return
		bb2->min.x >= bb->min.x && bb2->max.x <= bb->max.x &&
		bb2->min.y >= bb->min.y && bb2->max.y <= bb->max.y &&
		bb2->min.z >= bb->min.z && bb2->max.z <= bb->max.z;
}

#endif