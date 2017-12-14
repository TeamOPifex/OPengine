#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL

struct OPvertexArrayGL;
typedef struct OPvertexArrayGL OPvertexArrayGL;

#include "./Human/include/Rendering/API/OPvertexArrayAPI.h"

void OPvertexArrayAPIGLInit(OPvertexArrayAPI* vertexArray);

struct OPvertexArrayGL {
	ui32 Handle;
};

#endif