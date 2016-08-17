#pragma once

struct OPeffectGL;
typedef struct OPeffectGL OPeffectGL;

#include "./Human/include/Rendering/API/OPeffectAPI.h"

void OPeffectAPIGLInit(OPeffectAPI* effect);

struct OPeffectGL {
	ui32 Handle;
};