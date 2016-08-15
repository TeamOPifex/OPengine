#pragma once

struct OPfontKerning;
typedef struct OPfontKerning OPfontKerning;

#include "./Core/include/OPtypes.h"

struct OPfontKerning {
	OPchar charcode;
	OPfloat kerning;
};
