#pragma once
#include "Image.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Data/include/OPstream.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
	void OPimagePNG24WriteStream(ui8* imageData, OPint width, OPint height, ui8** data, ui32* dataSize);
	void OPimagePNG32WriteStream(ui8* imageData, OPint width, OPint height, ui8** data, ui32* dataSize);
	void OPimagePNGCreate24(ui8* imageData, OPint width, OPint height, i8* filename);
	void OPimagePNGCreate32(ui8* imageData, OPint width, OPint height, i8* filename);
	OPint OPimagePNGLoad(const OPchar* filename, OPtexture** image);
	OPint OPimagePNGLoadStream(OPstream* str, OPint offset, OPtexture** image);
	OPint OPimagePNGUnload(void* image);
#ifdef __cplusplus
}
#endif