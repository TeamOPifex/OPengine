#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL_ES

struct OPvertexArrayGLES;
typedef struct OPvertexArrayGLES OPvertexArrayGLES;

#include "./Human/include/Rendering/API/OPvertexArrayAPI.h"

void OPvertexArrayAPIGLESInit(OPvertexArrayAPI* vertexArray);

struct OPvertexArrayGLES {
	ui32 Handle;
};

#endif