#ifndef OPENGINE_HUMAN_RENDERING_SHADER_UNIFORM_GL
#define OPENGINE_HUMAN_RENDERING_SHADER_UNIFORM_GL

#include "./Human/include/Rendering/API/OPshaderUniformAPI.h"

void OPshaderUniformAPIGLInit(OPshaderUniformAPI* shaderUniform);

struct OPshaderUniformGL {
	ui32 Handle;
};

#endif