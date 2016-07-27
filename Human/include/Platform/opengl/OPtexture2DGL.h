#ifndef OPENGINE_HUMAN_RENDERING_TEXTURE2D_GL
#define OPENGINE_HUMAN_RENDERING_TEXTURE2D_GL

#include "./Human/include/Rendering/API/OPtexture2D.h"
#include "./Human/include/Platform/opengl/OPtextureAPIGL.h"

void OPtexture2DGLInit(OPtexture2D* texture2D);

struct OPtexture2DGL {
	OPtextureGL Texture;
};

#endif