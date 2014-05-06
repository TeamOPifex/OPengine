#ifndef OP_MATH_BOUNDINGBOX2D
#define OP_MATH_BOUNDINGBOX2D

#include "Vector2.h"

typedef struct {
	OPvec2 min;
	OPvec2 max;
} OPboundingBox2D;

inline OPboundingBox2D OPboundingBox2DCreate(OPvec2 min, OPvec2 max) {
	OPboundingBox2D tmp = {
		min,
		max
	};
	return tmp;
}

#endif