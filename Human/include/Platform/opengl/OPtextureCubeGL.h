#ifndef OPENGINE_HUMAN_RENDERING_TEXTURE_CUBE_GL
#define OPENGINE_HUMAN_RENDERING_TEXTURE_CUBE_GL

#include "./Human/include/Rendering/API/OPtextureCube.h"
#include "./Human/include/Platform/opengl/OPtextureGL.h"

void OPtextureCubeGLInit(OPtextureCube* textureCube);

struct OPtextureCubeGL {
	OPtextureGL Texture;
};

#endif