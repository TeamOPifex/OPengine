#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL

struct OPframeBufferDepthGL;
typedef struct OPframeBufferDepthGL OPframeBufferDepthGL;

#include "./Human/include/Rendering/API/OPframeBufferDepthAPI.h"

void OPframeBufferDepthAPIGLInit(OPframeBufferDepthAPI* frameBuffer);

struct OPframeBufferDepthGL {
	ui32 Handle;
};

#endif