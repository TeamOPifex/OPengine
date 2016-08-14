#pragma once

struct OPshaderUniformGL;
typedef struct OPshaderUniformGL OPshaderUniformGL;

#include "./Human/include/Rendering/API/OPshaderUniformAPI.h"

void OPshaderUniformAPIGLInit(OPshaderUniformAPI* shaderUniform);

struct OPshaderUniformGL {
	ui32 Handle;
};