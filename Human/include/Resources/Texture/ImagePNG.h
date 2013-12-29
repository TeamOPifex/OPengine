#pragma once
#include "Image.h"
#include "./Human/include/Rendering/OPtexture.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
	void OPimagePNGCreate24(ui8* imageData, OPint width, OPint height, i8* filename);
	void OPimagePNGCreate32(ui8* imageData, OPint width, OPint height, i8* filename);
	OPint OPimagePNGLoad(const OPchar* filename, OPtexture** image);
	OPint OPimagePNGUnload(void* image);
#ifdef __cplusplus
}
#endif