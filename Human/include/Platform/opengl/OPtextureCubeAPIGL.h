#pragma once

struct OPtextureCubeGL;
typedef struct OPtextureCubeGL OPtextureCubeGL;

#include "./Human/include/Rendering/API/OPtextureCubeAPI.h"

void OPtextureCubeAPIGLInit(OPtextureCubeAPI* textureCube);

struct OPtextureCubeGL {
	ui32 Handle;
};