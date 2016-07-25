#ifndef OPENGINE_HUMAN_RENDERING_INDEX_BUFFER_GL
#define OPENGINE_HUMAN_RENDERING_INDEX_BUFFER_GL

#include "./Human/include/Rendering/API/OPindexBufferAPI.h"

void OPindexBufferAPIGLInit(OPindexBufferAPI* indexBuffer);

struct OPindexBufferAPIGL {
	ui32 Handle;
};

#endif