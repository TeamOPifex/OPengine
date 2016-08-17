#pragma once

struct OPfontTextNode;
typedef struct OPfontTextNode OPfontTextNode;

#include "./Core/include/OPtypes.h"

struct OPfontTextNode {
	OPchar* Text;
	OPint Width;
};