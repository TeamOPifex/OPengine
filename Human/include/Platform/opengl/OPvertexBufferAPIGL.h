#pragma once

struct OPvertexBufferGL;
typedef struct OPvertexBufferGL OPvertexBufferGL;

#include "./Human/include/Rendering/API/OPvertexBufferAPI.h"

void OPvertexBufferAPIGLInit(OPvertexBufferAPI* vertexBuffer);

struct OPvertexBufferGL {
	ui32 Handle;
};