#pragma once

struct OPbox2D;
typedef struct OPbox2D OPbox2D;

#include "./Math/include/OPvec2.h"

struct OPbox2D {
	OPvec2 position;
	OPvec2 size;

	OPbox2D() { }
	OPbox2D(OPvec2 pos, OPvec2 size) {
		this->position = pos;
		this->size = size;
	}
};