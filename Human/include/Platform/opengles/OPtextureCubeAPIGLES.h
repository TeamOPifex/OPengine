#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL_ES

struct OPtextureCubeGLES;
typedef struct OPtextureCubeGLES OPtextureCubeGLES;

#include "./Human/include/Rendering/API/OPtextureCubeAPI.h"

void OPtextureCubeAPIGLESInit(OPtextureCubeAPI* textureCube);

struct OPtextureCubeGLES {
	ui32 Handle;
};

#endif