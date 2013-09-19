#pragma once

#include "./Math/include/Vector2.h"
#include "./Math/include/Vector3.h"

typedef struct {
	OPvec3 Position;
	OPvec3 Normal;
	OPvec3 Tangent;
	OPvec2 TexCoord;
} OPMvertex;