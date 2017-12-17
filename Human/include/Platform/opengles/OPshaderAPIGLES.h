#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL_ES

struct OPshaderGLES;
typedef struct OPshaderGLES OPshaderGLES;

#include "./Human/include/Rendering/API/OPshaderAPI.h"

void OPshaderAPIGLESInit(OPshaderAPI* shader);

struct OPshaderGLES {
	ui32 Handle;
	bool Compiled;
};

#endif