#pragma once

struct OPwindowGL;
typedef struct OPwindowGL OPwindowGL;

#include "./Human/include/Rendering/API/OPwindowAPI.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"

void OPwindowAPIGLInit(OPwindowAPI* window);

struct OPwindowGL {
#ifndef OPIFEX_OPENGL_ES_2
	GLFWwindow* Handle;
#endif

	void(*OPWINDOWDROPCALLBACK)(OPuint count, const OPchar**) = NULL;
};