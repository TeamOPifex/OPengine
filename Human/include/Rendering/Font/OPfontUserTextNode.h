#pragma once

struct OPfontUserTextNode;
typedef struct OPfontUserTextNode OPfontUserTextNode;

#include "./Human/include/Rendering/OPmodel.h"

struct OPfontUserTextNode {
	OPmodel mesh;
	OPfloat Width;
};