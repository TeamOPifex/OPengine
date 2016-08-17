#pragma once

struct OPray2D;
typedef struct OPray2D OPray2D;

#include "./Math/include/OPvec2.h"

struct OPray2D {
	OPvec2 position;
	OPvec2 direction;
};