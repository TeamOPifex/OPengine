#pragma once

#include "./Math/include/Vector2.h"
#include "./Math/include/Vector3.h"

typedef struct {
	OPvec3 vertice;
	OPvec3 normal;
	OPvec2 tex;
	OPvec3 tangent;
} OPMvertex;