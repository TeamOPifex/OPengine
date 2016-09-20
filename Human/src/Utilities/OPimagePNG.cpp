
#include "./Human/include/Utilities/OPimagePNG.h"
#include "./External/LodePNG/include/LodePNG.h"
#include "./Human/include/Rendering/OPimage.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPstring.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/Assert.h"
#include "./Core/include/OPmemory.h"
#include <stdio.h>
#include <string.h>
#include <cstdio>
#include <string>

//
//#ifdef OPIFEX_OPENGL_ES_2
//    #ifdef OPIFEX_IOS
//    #include <OpenGLES/ES2/gl.h>
//    #else
//    #include <GLES2/gl2.h>
//    #include <GLES2/gl2ext.h>
//    #endif
//
//#else
//    #include <GLFW/glfw3.h>
//    #include <GL/glew.h>
//#endif

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

OPimage OPimagePNGLoadData(const OPchar* filename) {
	ui32 error;
	ui8* data;
	ui32 width, height;
	OPimage result = { 0, 0, 0 };
	OPstream* str = OPfile::ReadFromFile(filename);
	ASSERT(str != NULL, "FAILED to read file");

	error = lodepng_decode32(&data, &width, &height, str->Data, str->Length);
	if(error) {
		OPlog("LodePNG Error %d", error);
		ASSERT(false, "FAILED to load PNG data");
	}

	result.Width = width;
	result.Height = height;
	result.Data = data;

	str->Destroy();
	OPfree(str);

	return result;	
}

#include "./Core/include/OPdebug.h"

i32 OPimagePNGLoadStream(OPstream* str, OPuint offset, OPtexture** image, OPtextureFilter filter) {
	TIMED_BLOCK;
	if (str == NULL) {
		return 0;
	}

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
	OPtexture* tex = OPNEW(OPtexture());

	ui16 w = width, h = height;
	OPtextureDesc desc;
	desc.width = w;
	desc.height = h;
	desc.format = OPtextureFormat::RGBA;
	desc.wrap = OPtextureWrap::REPEAT;
	desc.filter = filter;

	OPRENDERER_ACTIVE->Texture.Init(tex, desc);
	tex->SetData(data);

#ifdef _DEBUG
	tex->source = OPstringCopy(str->Source);
#endif

	// clean up
	free(data); // Clean up load png 

	*image = tex;


	return 1;
}

i32 OPimagePNGLoadStream(OPstream* str, OPuint offset, OPtexture** image) {
	OPimagePNGLoadStream(str, offset, image, OPtextureFilter::LINEAR);
	return 1;
}