#pragma once

#include "Vector3.h"

typedef struct {
	OPvec3 cols[3];
} OPmat3;

inline void OPmat3identity(OPmat3* dst);