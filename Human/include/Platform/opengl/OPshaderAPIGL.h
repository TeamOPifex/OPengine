#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL

struct OPshaderGL;
typedef struct OPshaderGL OPshaderGL;

#include "./Human/include/Rendering/API/OPshaderAPI.h"

void OPshaderAPIGLInit(OPshaderAPI* shader);

struct OPshaderGL {
	ui32 Handle;
	bool Compiled;
};

#endif