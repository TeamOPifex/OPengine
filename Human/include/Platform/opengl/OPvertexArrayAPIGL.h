#ifndef OPENGINE_HUMAN_RENDERING_VERTEX_ARRAY_GL
#define OPENGINE_HUMAN_RENDERING_VERTEX_ARRAY_GL

#include "./Human/include/Rendering/API/OPvertexArrayAPI.h"

void OPvertexArrayAPIGLInit(OPvertexArrayAPI* vertexArray);

struct OPvertexArrayGL {
	ui32 Handle;
};

#endif