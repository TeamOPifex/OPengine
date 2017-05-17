#pragma once

struct OPsphere;
typedef struct OPsphere OPsphere;

#include "./Human/include/Rendering/OPmodel.h"

struct OPsphere {
	static OPvec3 Color;
	static OPmodel* Create(ui32 depth, ui32 features);
};