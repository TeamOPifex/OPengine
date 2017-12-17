#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL_ES

struct OPeffectGLES;
typedef struct OPeffectGLES OPeffectGLES;

#include "./Human/include/Rendering/API/OPeffectAPI.h"

void OPeffectAPIGLESInit(OPeffectAPI* effect);

struct OPeffectGLES {
	ui32 Handle;
};

#endif