#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL_ES

struct OPframeBufferGLES;
typedef struct OPframeBufferGLES OPframeBufferGLES;

#include "./Human/include/Rendering/API/OPframeBufferAPI.h"

void OPframeBufferAPIGLESInit(OPframeBufferAPI* frameBuffer);

struct OPframeBufferGLES {
	ui32 Handle;
	ui32 DepthHandle;
};

#endif