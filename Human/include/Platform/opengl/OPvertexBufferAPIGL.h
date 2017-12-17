#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL

struct OPvertexBufferGL;
typedef struct OPvertexBufferGL OPvertexBufferGL;

#include "./Human/include/Rendering/API/OPvertexBufferAPI.h"

void OPvertexBufferAPIGLInit(OPvertexBufferAPI* vertexBuffer);

struct OPvertexBufferGL {
	ui32 Handle;
};

#endif