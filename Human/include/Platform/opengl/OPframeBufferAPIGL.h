#pragma once

struct OPframeBuffer2DGL;
typedef struct OPframeBuffer2DGL OPframeBuffer2DGL;

#include "./Human/include/Rendering/API/OPframeBufferAPI.h"

void OPframeBufferAPIGLInit(OPframeBufferAPI* frameBuffer);

struct OPframeBufferGL {
	ui32 Handle;
	ui32 DepthHandle;
};