#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL_ES

struct OPwindowGLES;
typedef struct OPwindowGLES OPwindowGLES;

#include "./Human/include/Rendering/API/OPwindowAPI.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"

void OPwindowAPIGLESInit(OPwindowAPI* window);

struct OPwindowGLES {
	void(*OPWINDOWDROPCALLBACK)(OPuint count, const OPchar**) = NULL;
};

#endif