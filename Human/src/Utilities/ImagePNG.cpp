#include "./Human/include/Utilities/ImagePNG.h"

#include <string.h>
#include "./Core/include/Log.h"
#include "./Data/include/File.h"
#include <cstdio>
#include <string>
#include "./Core/include/DynamicMemory.h"
#include "./Human/include/Utilities/LodePNG.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Utilities/Errors.h"

#if defined(OPIFEX_OPENGL_ES_2)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

void OPimagePNG24WriteStream(ui8* imageData, i32 width, i32 height, ui8** data, OPuint* dataSize) {
	ui32 error = lodepng_encode24(data, dataSize, imageData, width, height);
}
void OPimagePNG32WriteStream(ui8* imageData, i32 width, i32 height, ui8** data, OPuint* dataSize) {
	ui32 error = lodepng_encode32(data, dataSize, imageData, width, height);
}

void OPimagePNGCreate24(ui8* imageData, i32 width, i32 height, i8* filename) {
	ui8*   data;
	OPuint dataSize;
	ui32 error = lodepng_encode24(&data, &dataSize, imageData, width, height);
	lodepng_save_file(data, dataSize, filename);
}

void OPimagePNGCreate32(ui8* imageData, i32 width, i32 height, i8* filename) {
	ui8*   data;
	OPuint dataSize;
	ui32 error = lodepng_encode32(&data, &dataSize, imageData, width, height);
	lodepng_save_file(data, dataSize, filename);
}

i32 OPimagePNGLoad(const OPchar* filename, OPtexture** image){
	CheckError("OPimagePNGLoad:Error 0");
	ui32 error;
	ui8* data;
	ui32 width, height;
	OPstream* str = OPreadFile(filename);
	return OPimagePNGLoadStream(str, 0, image);
}


i32 OPimagePNGLoadStream(OPstream* str, i32 offset, OPtexture** image) {
	CheckError("OPimagePNGLoad:Error 0");
	ui32 error;
	ui8* data;
	ui32 width, height;
	error = lodepng_decode32(&data, &width, &height, str->Data + offset, str->Length - offset);
	if (error) {
		OPlog("LodePNG Error %d", error);
	}
	CheckError("OPimagePNGLoad:Error 1");
	OPtexture* tex = (OPtexture*)OPalloc(sizeof(OPtexture));
	CheckError("OPimagePNGLoad:Error 2");

	OPtextureDescription desc = {
		width,
		height,
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		OPtextureLinear,
		OPtextureLinear,
		OPtextureRepeat,
		OPtextureRepeat
	};

	CheckError("OPimagePNGLoad:Error 3");
	*tex = OPtextureCreate(desc);
	CheckError("OPimagePNGLoad:Error 4");
	OPtextureBind(tex);
	CheckError("OPimagePNGLoad:Error 5");
	OPtextureSetData(data);
	CheckError("OPimagePNGLoad:Error 6");

	// clean up
	OPfree(data);
	OPstreamDestroy(str);

	*image = tex;

	CheckError("OPimagePNGLoad:Error 7");

	return 1;
}

i32 OPimagePNGUnload(void* image){
	OPtexture* tex = (OPtexture*)image;
	OPtextureDestroy(tex);
	OPfree(image);

	return 1;
}
