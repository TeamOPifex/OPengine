#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL

struct OPtextureCubeGL;
typedef struct OPtextureCubeGL OPtextureCubeGL;

#include "./Human/include/Rendering/API/OPtextureCubeAPI.h"

void OPtextureCubeAPIGLInit(OPtextureCubeAPI* textureCube);

struct OPtextureCubeGL {
	ui32 Handle;
};

#endif