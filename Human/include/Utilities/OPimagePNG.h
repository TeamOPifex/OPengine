#ifndef OPENGINE_HUMAN_UTILITIES_IMAGE_PNG
#define OPENGINE_HUMAN_UTILITIES_IMAGE_PNG

#include "./Human/include/Rendering/OPtexture.h"
#include "./Data/include/OPstream.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
	struct OPimagePNG {
		ui32 Width;
		ui32 Height;
		ui8* Data;
	};

	void OPimagePNG24WriteStream(ui8* imageData, i32 width, i32 height, ui8** data, OPuint* dataSize);
	void OPimagePNG32WriteStream(ui8* imageData, i32 width, i32 height, ui8** data, OPuint* dataSize);
	void OPimagePNGCreate24(ui8* imageData, i32 width, i32 height, OPchar* filename);
	void OPimagePNGCreate32(ui8* imageData, i32 width, i32 height, OPchar* filename);
	i32 OPimagePNGLoad(OPstream* str, OPtexture** image);
	i32 OPimagePNGLoadStream(OPstream* str, i32 offset, OPtexture** image);
	OPimagePNG OPimagePNGLoadData(const OPchar* filename);
	i32 OPimagePNGReload(OPstream* str, OPtexture** image);
	i32 OPimagePNGUnload(void* image);
#ifdef __cplusplus
}
#endif

#endif
