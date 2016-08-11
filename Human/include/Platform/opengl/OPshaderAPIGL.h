#ifndef OPENGINE_HUMAN_PLATFORM_SHADER_GL
#define OPENGINE_HUMAN_PLATFORM_SHADER_GL

#include "./Human/include/Rendering/API/OPshaderAPI.h"

void OPshaderAPIGLInit(OPshaderAPI* shader);

struct OPshaderGL {
	ui32 Handle;
	bool Compiled;
};

#endif