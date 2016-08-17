#pragma once

struct OPshaderAttributeGL;
typedef struct OPshaderAttributeGL OPshaderAttributeGL;

#include "./Human/include/Rendering/API/OPshaderAttributeAPI.h"

void OPshaderAttributeAPIGLInit(OPshaderAttributeAPI* shaderAttribute);

struct OPshaderAttributeGL {
	ui32 Handle;
};