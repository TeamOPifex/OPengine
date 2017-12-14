#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL

struct OPshaderAttributeGL;
typedef struct OPshaderAttributeGL OPshaderAttributeGL;

#include "./Human/include/Rendering/API/OPshaderAttributeAPI.h"

void OPshaderAttributeAPIGLInit(OPshaderAttributeAPI* shaderAttribute);

struct OPshaderAttributeGL {
	ui32 Handle;
};

#endif