#ifndef OPENGINE_HUMAN_UTILITIES_IMAGE_PNG
#define OPENGINE_HUMAN_UTILITIES_IMAGE_PNG

#include "./Human/include/Rendering/Texture.h"
#include "./Data/include/Stream.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
	void OPimagePNG24WriteStream(ui8* imageData, i32 width, i32 height, ui8** data, OPuint* dataSize);
	void OPimagePNG32WriteStream(ui8* imageData, i32 width, i32 height, ui8** data, OPuint* dataSize);
	void OPimagePNGCreate24(ui8* imageData, i32 width, i32 height, OPchar* filename);
	void OPimagePNGCreate32(ui8* imageData, i32 width, i32 height, OPchar* filename);
	i32 OPimagePNGLoad(const OPchar* filename, OPtexture** image);
	i32 OPimagePNGLoadStream(OPstream* str, i32 offset, OPtexture** image);
	i32 OPimagePNGUnload(void* image);
#ifdef __cplusplus
}
#endif

#endif
