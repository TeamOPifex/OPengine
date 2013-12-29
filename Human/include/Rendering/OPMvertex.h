#pragma once

#include "./Math/include/Vector2.h"
#include "./Math/include/Vector3.h"
#include "./Math/include/Vector4.h"

typedef struct {
	OPvec3 Position;
	OPvec3 Normal;
	OPvec3 Tangent;
	OPvec2 TexCoord;
} OPMvertex;

typedef struct {
	OPvec3 Position;
	OPvec2 TexCoord;
} OPvertexColor;