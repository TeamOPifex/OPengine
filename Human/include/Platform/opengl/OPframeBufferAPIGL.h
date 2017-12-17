#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL

struct OPframeBufferGL;
typedef struct OPframeBufferGL OPframeBufferGL;

#include "./Human/include/Rendering/API/OPframeBufferAPI.h"

void OPframeBufferAPIGLInit(OPframeBufferAPI* frameBuffer);

struct OPframeBufferGL {
	ui32 Handle;
	ui32 DepthHandle;
};

#endif