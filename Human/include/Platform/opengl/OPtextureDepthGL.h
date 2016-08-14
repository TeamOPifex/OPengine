#pragma once

struct OPtextureDepthGL;
typedef struct OPtextureDepthGL OPtextureDepthGL;

#include "./Human/include/Rendering/API/OPtextureDepth.h"
#include "./Human/include/Platform/opengl/OPtextureAPIGL.h"

void OPtextureDepthGLInit(OPtextureDepth* textureDept);

struct OPtextureDepthGL {
	OPtextureGL Texture;	
};