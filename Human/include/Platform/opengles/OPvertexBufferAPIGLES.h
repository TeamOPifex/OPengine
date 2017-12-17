#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL_ES

struct OPvertexBufferGLES;
typedef struct OPvertexBufferGLES OPvertexBufferGLES;

#include "./Human/include/Rendering/API/OPvertexBufferAPI.h"

void OPvertexBufferAPIGLESInit(OPvertexBufferAPI* vertexBuffer);

struct OPvertexBufferGLES {
	ui32 Handle;
};

#endif