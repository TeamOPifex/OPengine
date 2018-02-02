#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL

struct OPshaderUniformBufferGL;
typedef struct OPshaderUniformBufferGL OPshaderUniformBufferGL;

#include "./Human/include/Rendering/API/OPshaderUniformBufferAPI.h"

void OPshaderUniformBufferAPIGLInit(OPshaderUniformBufferAPI* shaderUniformBuffer);

struct OPshaderUniformBufferGL {
	ui32 Handle;
};

#endif