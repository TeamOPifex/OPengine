#pragma once
#include "Image.h"
#include "./Human/include/Rendering/OPtexture.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
	OPint OPimagePNGLoad(const OPchar* filename, OPtexture** image);
	OPint OPimagePNGUnload(void* image);
#ifdef __cplusplus
}
#endif