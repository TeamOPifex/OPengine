#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL_ES

struct OPframeBufferDepthGLES;
typedef struct OPframeBufferDepthGLES OPframeBufferDepthGLES;

#include "./Human/include/Rendering/API/OPframeBufferDepthAPI.h"

void OPframeBufferDepthAPIGLESInit(OPframeBufferDepthAPI* frameBuffer);

struct OPframeBufferDepthGLES {
	ui32 Handle;
};

#endif