#ifndef OPENGINE_HUMAN_PLATFORM_VERTEXBUFFER_GL
#define OPENGINE_HUMAN_PLATFORM_VERTEXBUFFER_GL

#include "./Human/include/Rendering/API/OPvertexBufferAPI.h"
#include "./Human/include/Rendering/OPvertexBuffer.h"

void OPvertexBufferAPIGLInit(OPvertexBufferAPI* vertexBuffer);

struct OPvertexBufferAPIGL {
	ui32 Handle;
};

#endif