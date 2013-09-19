#pragma once

#include "./Core/include/Types.h"

typedef struct {
	OPfloat x, y, z, w;
} OPvec4;

inline void OPvec4norm(OPvec4* dst, OPvec4* a);