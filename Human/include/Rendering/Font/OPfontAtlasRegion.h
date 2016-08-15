#pragma once

struct OPfontAtlasRegion;
typedef struct OPfontAtlasRegion OPfontAtlasRegion;

#include "./Core/include/OPtypes.h"

struct OPfontAtlasRegion {
	i32 x;
	i32 y;
	i32 width;
	i32 height;
};