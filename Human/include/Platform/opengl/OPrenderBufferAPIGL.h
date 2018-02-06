#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL

struct OPrenderBufferGL;
typedef struct OPrenderBufferGL OPrenderBufferGL;

#include "./Human/include/Rendering/API/OPrenderBufferAPI.h"

void OPrenderBufferAPIGLInit(OPrenderBufferAPI* renderBuffer);

struct OPrenderBufferGL {
	ui32 Handle;

};

#endif