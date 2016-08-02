#ifndef OPENGINE_HUMAN_PLATFORM_WINDOW_GL
#define OPENGINE_HUMAN_PLATFORM_WINDOW_GL

#include "./Human/include/Rendering/API/OPwindowAPI.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"

void OPwindowAPIGLInit(OPwindowAPI* window);

struct OPwindowGL {
	GLFWwindow* Handle;
};

#endif