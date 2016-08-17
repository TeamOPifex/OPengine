#pragma once

struct OPtexture2DGL;
typedef struct OPtexture2DGL OPtexture2DGL;

#include "./Human/include/Platform/opengl/OPtextureAPIGL.h"
#include "./Human/include/Rendering/API/OPtexture2D.h"

void OPtexture2DGLInit(OPtexture2D* texture2D);

struct OPtexture2DGL {
	OPtextureGL Texture;
};