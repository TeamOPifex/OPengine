#ifndef OPENGINE_HUMAN_RENDERING_TEXTURE_GL
#define OPENGINE_HUMAN_RENDERING_TEXTURE_GL

#include "./Human/include/Rendering/API/OPtexture.h"

void OPtextureGLInit(OPtexture* texture);

struct OPtextureGL {
	ui32 Handle;

	OPtextureWrap TextureWrap;
	OPtextureFilter TextureFilter;
};

#endif