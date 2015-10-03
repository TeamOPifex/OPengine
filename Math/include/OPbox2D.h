#ifndef OP_MATH_BOX2D
#define OP_MATH_BOX2D

#include "./Math/include/OPvec2.h"

struct OPbox2D {
	OPvec2 position;
	OPvec2 size;
};

inline OPbox2D OPbox2DCreate(OPvec2 pos, OPvec2 size) {
	OPbox2D tmp = {
		pos,
		size
	};
	return tmp;
}

#endif
