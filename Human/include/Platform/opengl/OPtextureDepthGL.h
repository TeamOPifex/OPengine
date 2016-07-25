#ifndef OPENGINE_HUMAN_RENDERING_TEXTURE_DEPTH_GL
#define OPENGINE_HUMAN_RENDERING_TEXTURE_DEPTH_GL

#include "./Human/include/Rendering/API/OPtextureDepth.h"
#include "./Human/include/Platform/opengl/OPtextureGL.h"

void OPtextureDepthGLInit(OPtextureDepth* textureDept);

struct OPtextureDepthGL {
	OPtextureGL Texture;	
};

#endif