#ifndef OPENGINE_HUMAN_PLATFORM_EFFECT_GL
#define OPENGINE_HUMAN_PLATFORM_EFFECT_GL

#include "./Human/include/Rendering/API/OPeffectAPI.h"
#include "./Human/include/Rendering/OPeffect.h"

void OPeffectAPIGLInit(OPeffectAPI* effect);

struct OPeffectGL {
	ui32 Handle;
};

#endif