#include "./Human/include/Utilities/OPimagePNG.h"

#include <string.h>
#include "./Core/include/OPlog.h"
#include "./Core/include/Assert.h"
#include "./Data/include/OPfile.h"
#include <cstdio>
#include <string>
#include "./Core/include/OPmemory.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Utilities/Errors.h"
#include "./External/LodePNG/include/LodePNG.h"


#ifdef OPIFEX_OPENGL_ES_2
    #ifdef OPIFEX_IOS
    #include <OpenGLES/ES2/gl.h>
    #else
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    #endif

#else
    #include <GLFW/glfw3.h>
    #include <GL/glew.h>
#endif

void OPimagePNG24WriteStream(ui8* imageData, i32 width, i32 height, ui8** data, OPuint* dataSize) {
	ui32 error = lodepng_encode24(data, (size_t*)dataSize, imageData, width, height);
}
void OPimagePNG32WriteStream(ui8* imageData, i32 width, i32 height, ui8** data, OPuint* dataSize) {
	ui32 error = lodepng_encode32(data, (size_t*)dataSize, imageData, width, height);
}

void OPimagePNGCreate24(ui8* imageData, i32 width, i32 height, OPchar* filename) {
	ui8*   data;
	size_t dataSize;
	ui32 error = lodepng_encode24(&data, &dataSize, imageData, width, height);
	lodepng_save_file(data, dataSize, filename);
}

void OPimagePNGCreate32(ui8* imageData, i32 width, i32 height, OPchar* filename) {
	ui8*   data;
	size_t dataSize;
	ui32 error = lodepng_encode32(&data, &dataSize, imageData, width, height);
	lodepng_save_file(data, dataSize, filename);
}

i32 OPimagePNGLoad(OPstream* str, OPtexture** image){
	OPlog("OPimagePNGLoad image %s", str->Source);
	OPglError("OPimagePNGLoad:Error 0::%d");
	//OPstream* str = OPreadFile(filename);
	ASSERT(str != NULL, "Image not found.");
	return OPimagePNGLoadStream(str, 0, image);
}

OPimagePNG OPimagePNGLoadData(const OPchar* filename) {
	ui32 error;
	ui8* data;
	ui32 width, height;
	OPimagePNG result = { 0, 0, 0 };
	OPstream* str = OPreadFile(filename);
	ASSERT(str != NULL, "FAILED to read file");

	error = lodepng_decode32(&data, &width, &height, str->Data, str->Length);
	if(error) {
		OPlog("LodePNG Error %d", error);
		ASSERT(false, "FAILED to load PNG data");
	}

	result.Width = width;
	result.Height = height;
	result.Data = data;

	return result;	
}

i32 OPimagePNGLoadStream(OPstream* str, i32 offset, OPtexture** image) {
	OPglError("OPimagePNGLoad:Error 0");
	ui32 error;
	ui8* data;
	ui32 width, height;
	error = lodepng_decode32(&data, &width, &height, str->Data + offset, str->Length - offset);
	if (error) {
		OPlog("LodePNG Error %d", error);
	}
	//OPlog("Width: %d, Height: %d", width, height);
	//for (i32 i = 0; i < width * height; i++) {
	//	OPlog("Data: %s", data[i]);
	//	if (data[i] == 0) break;
	//}
	OPglError("OPimagePNGLoad:Error 1");
	OPtexture* tex = (OPtexture*)OPalloc(sizeof(OPtexture));
	OPglError("OPimagePNGLoad:Error 2");

	ui16 w = width, h = height;
	OPtextureDescription desc = {
		w,
		h,
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		OPtextureLinear,
		OPtextureLinear,
		OPtextureRepeat,
		OPtextureRepeat
	};

	OPtextureClearActive();
	OPglError("OPimagePNGLoad:Error 3");
	*tex = OPtextureCreate(desc);
	OPglError("OPimagePNGLoad:Error 4");
	OPtextureBind(tex);
	OPglError("OPimagePNGLoad:Error 5");
	OPtextureSetData(data);
	OPglError("OPimagePNGLoad:Error 6");

	// clean up
	OPfree(data);
	//OPstreamDestroy(str);

	*image = tex;

	OPglError("OPimagePNGLoad:Error 7");

	return 1;
}

i32 OPimagePNGReload(OPstream* str, OPtexture** image){
	OPlog("Reload Image PNG");
	//OPstream* str = OPreadFile(filename);
	OPtexture* resultTex;
	OPtexture* tex = (OPtexture*)(*image);
	OPint result = OPimagePNGLoadStream(str, 0, &resultTex);
	if (result) {
		OPmemcpy(*image, resultTex, sizeof(OPtexture));
		OPfree(resultTex);
	}
	return result;
}

i32 OPimagePNGUnload(void* image){
	OPtexture* tex = (OPtexture*)image;
	OPtextureDestroy(tex);
	OPfree(tex);

	return 1;
}
