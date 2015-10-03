#ifndef OP_MATH_LINE2D
#define OP_MATH_LINE2D

#include "./Math/include/OPvec2.h"

struct OPline2D {
	OPvec2 start;
	OPvec2 end;
};

inline OPline2D OPline2DCreate(OPvec2 start, OPvec2 end) {
	OPline2D tmp = {
		start,
		end
	};
	return tmp;
}

#endif
