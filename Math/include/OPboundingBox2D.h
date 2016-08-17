#pragma once

struct OPboundingBox2D;
typedef struct OPboundingBox2D OPboundingBox2D;

#include "./Math/include/OPvec2.h"

struct OPboundingBox2D {
	OPvec2 min;
	OPvec2 max;

	OPboundingBox2D() { }
	OPboundingBox2D(OPvec2 min, OPvec2 max) {
		this->min = min;
		this->max = max;
	}
};

