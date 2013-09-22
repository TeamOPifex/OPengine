#include "./Human/include/Resources/Texture/ImagePNG.h"

#include <string.h>
#include "./Core/include/Log.h"
#include "./Data/include/OPfile.h"
#include <cstdio>
#include <string>
#include "./Core/include/DynamicMemory.h"
#include "./Human/include/Utilities/LodePNG.h"
#include "./Human/include/Rendering/OPtexture.h"


#if defined(OPIFEX_OPENGL_ES_2)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#include <GL/glfw.h>
#endif

OPint OPimagePNGLoad(const OPchar* filename, OPtexture** image){
	ui32 error;
	ui8* data;
	ui32 width, height;
	error = lodepng_decode32_file(&data, &width, &height, filename);

	OPtexture* tex = (OPtexture*)OPalloc(sizeof(OPtexture));

	OPtextureDescription desc = {
		width,
		height,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		OPtextureLinear,
		OPtextureLinear,
		OPtextureRepeat,
		OPtextureRepeat
	};

	*tex = OPtextureCreate(desc);
	OPtextureBind(tex, 0);
	OPtextureSetData(data);

	*image = tex;

	return 1;
}

OPint OPimagePNGUnload(void* image){
	// TODO
	return 1;
}