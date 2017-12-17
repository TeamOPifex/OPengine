#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL

struct OPshaderUniformGL;
typedef struct OPshaderUniformGL OPshaderUniformGL;

#include "./Human/include/Rendering/API/OPshaderUniformAPI.h"

void OPshaderUniformAPIGLInit(OPshaderUniformAPI* shaderUniform);

struct OPshaderUniformGL {
	ui32 Handle;
};

#endif