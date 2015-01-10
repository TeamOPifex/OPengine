#ifndef OPENGINE_HUMAN_UTILITIES_IMAGE_PNG
#define OPENGINE_HUMAN_UTILITIES_IMAGE_PNG

#include "./Human/include/Rendering/OPtexture.h"
#include "./Data/include/OPstream.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
	EXPORT_METHOD void OPimagePNG24WriteStream(ui8* imageData, i32 width, i32 height, ui8** data, OPuint* dataSize);
	EXPORT_METHOD void OPimagePNG32WriteStream(ui8* imageData, i32 width, i32 height, ui8** data, OPuint* dataSize);
	EXPORT_METHOD void OPimagePNGCreate24(ui8* imageData, i32 width, i32 height, OPchar* filename);
	EXPORT_METHOD void OPimagePNGCreate32(ui8* imageData, i32 width, i32 height, OPchar* filename);
	EXPORT_METHOD i32 OPimagePNGLoad(const OPchar* filename, OPtexture** image);
	EXPORT_METHOD i32 OPimagePNGLoadStream(OPstream* str, i32 offset, OPtexture** image);
	EXPORT_METHOD i32 OPimagePNGReload(const OPchar* filename, OPtexture** image);
	EXPORT_METHOD i32 OPimagePNGUnload(void* image);
#ifdef __cplusplus
}
#endif

#endif
