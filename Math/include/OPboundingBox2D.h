#ifndef OP_MATH_BOUNDINGBOX2D
#define OP_MATH_BOUNDINGBOX2D

#include "./Math/include/OPvec2.h"

struct OPboundingBox2D {
	OPvec2 min;
	OPvec2 max;
};

/* Creates an OPboundingBox2D
 * @param min Minimum bounds of the bounding box to create
 * @param max Maximum bounds of the bounding box to create
 * @return the resulting box
*/
inline OPboundingBox2D OPboundingBox2DCreate(OPvec2 min, OPvec2 max) {
	OPboundingBox2D tmp = {
		min,
		max
	};
	return tmp;
}

#endif
