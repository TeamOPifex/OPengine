#ifndef OP_MATH_BOX2D
#define OP_MATH_BOX2D

#include "../include/Vector2.h"

typedef struct {
	OPvec2 position;
	OPvec2 size;
} OPbox2D;

inline OPbox2D OPbox2DCreate(OPvec2 pos, OPvec2 size) {
	OPbox2D tmp = {
		pos,
		size
	};
	return tmp;
}

#endif