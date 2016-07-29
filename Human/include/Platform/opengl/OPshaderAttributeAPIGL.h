#ifndef OPENGINE_HUMAN_PLATFORM_SHADER_ATTRIBUTE_GL
#define OPENGINE_HUMAN_PLATFORM_SHADER_ATTRIBUTE_GL

#include "./Human/include/Rendering/API/OPshaderAttributeAPI.h"

void OPshaderAttributeAPIGLInit(OPshaderAttributeAPI* shaderAttribute);

struct OPshaderAttributeGL {
	ui32 Handle;
};

#endif