#pragma once

struct OPvertexArrayGL;
typedef struct OPvertexArrayGL OPvertexArrayGL;

#include "./Human/include/Rendering/API/OPvertexArrayAPI.h"

void OPvertexArrayAPIGLInit(OPvertexArrayAPI* vertexArray);

struct OPvertexArrayGL {
	ui32 Handle;
};