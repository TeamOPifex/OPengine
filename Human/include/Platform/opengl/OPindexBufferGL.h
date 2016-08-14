#pragma once

struct OPindexBufferAPIGL;
typedef struct OPindexBufferAPIGL OPindexBufferAPIGL;

#include "./Human/include/Rendering/API/OPindexBufferAPI.h"

void OPindexBufferAPIGLInit(OPindexBufferAPI* indexBuffer);

struct OPindexBufferAPIGL {
	ui32 Handle;
};