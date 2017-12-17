#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL_ES

struct OPshaderAttributeGLES;
typedef struct OPshaderAttributeGLES OPshaderAttributeGLES;

#include "./Human/include/Rendering/API/OPshaderAttributeAPI.h"

void OPshaderAttributeAPIGLESInit(OPshaderAttributeAPI* shaderAttribute);

struct OPshaderAttributeGLES {
	ui32 Handle;
};

#endif