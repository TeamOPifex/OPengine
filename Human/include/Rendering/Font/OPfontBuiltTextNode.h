#pragma once

struct OPfontBuiltTextNode;
typedef struct OPfontBuiltTextNode OPfontBuiltTextNode;

#include "./Human/include/Rendering/OPmeshPacked.h"

struct OPfontBuiltTextNode {
	OPmeshPacked* packedMesh;
	OPfloat Width;
};