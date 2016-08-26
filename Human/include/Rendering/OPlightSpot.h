#pragma once

struct OPlightSpot;
typedef struct OPlightSpot OPlightSpot;

#include "./Math/include/OPvec3.h"

struct OPlightSpot {
	OPvec3 position;
	OPvec3 color;
	OPfloat linear;
	OPfloat quadratic;
	OPfloat radius;
};