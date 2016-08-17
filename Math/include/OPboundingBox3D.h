#pragma once

struct OPboundingBox3D;
typedef struct OPboundingBox3D OPboundingBox3D;

#include "./Math/include/OPvec3.h"
#include "./Math/include/OPintersectionType.h"

struct OPray3D;

struct OPboundingBox3D {
	OPvec3 min;
	OPvec3 max;

	OPboundingBox3D() { }
	OPboundingBox3D(OPvec3 min, OPvec3 max) {
		this->min = min;
		this->max = max;
	}

	OPintersectionType Collision(OPboundingBox3D bounds);
	bool Intersects(OPray3D ray);

	inline bool Contains(OPvec3* point) {
		return
			point->x >= min.x && point->x <= max.x &&
			point->y >= min.y && point->y <= max.y &&
			point->z >= min.z && point->z <= max.z;
	}

	inline OPint Contains(OPboundingBox3D* bb2) {
		return
			bb2->min.x >= min.x && bb2->max.x <= max.x &&
			bb2->min.y >= min.y && bb2->max.y <= max.y &&
			bb2->min.z >= min.z && bb2->max.z <= max.z;
	}

	inline void Translate(OPvec3 point) {
		min += point;
		max += point;
	}
};