#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL_ES

struct OPindexBufferAPIGLES;
typedef struct OPindexBufferAPIGLES OPindexBufferAPIGLES;

#include "./Human/include/Rendering/API/OPindexBufferAPI.h"

void OPindexBufferAPIGLESInit(OPindexBufferAPI* indexBuffer);

struct OPindexBufferAPIGLES {
	ui32 Handle;
};

#endif