#pragma once

struct OPfontUserTextNode;
typedef struct OPfontUserTextNode OPfontUserTextNode;

#include "./Human/include/Rendering/OPmesh.h"

struct OPfontUserTextNode {
	OPmesh mesh;
	OPfloat Width;
};