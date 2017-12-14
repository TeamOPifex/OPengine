#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL

struct OPindexBufferAPIGL;
typedef struct OPindexBufferAPIGL OPindexBufferAPIGL;

#include "./Human/include/Rendering/API/OPindexBufferAPI.h"

void OPindexBufferAPIGLInit(OPindexBufferAPI* indexBuffer);

struct OPindexBufferAPIGL {
	ui32 Handle;
};

#endif