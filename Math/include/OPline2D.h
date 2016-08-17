#pragma once

struct OPline2D;
typedef struct OPline2D OPline2D;

#include "./Math/include/OPvec2.h"
#include "./Math/include/OPintersectionType.h"
struct OPboundingBox2D;

struct OPline2D {
	OPvec2 start;
	OPvec2 end;

	OPline2D() { }
	OPline2D(OPvec2 start, OPvec2 end) {
		this->start = start;
		this->end = end;
	}

	OPintersectionType Intersect(OPboundingBox2D* bb);
};