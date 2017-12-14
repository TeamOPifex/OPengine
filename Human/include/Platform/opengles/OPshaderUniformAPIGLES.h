#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL_ES

struct OPshaderUniformGLES;
typedef struct OPshaderUniformGLES OPshaderUniformGLES;

#include "./Human/include/Rendering/API/OPshaderUniformAPI.h"

void OPshaderUniformAPIGLESInit(OPshaderUniformAPI* shaderUniform);

struct OPshaderUniformGLES {
	ui32 Handle;
};

#endif