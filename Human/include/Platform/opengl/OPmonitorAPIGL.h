#ifndef OPENGINE_HUMAN_PLATFORM_MONITOR_GL
#define OPENGINE_HUMAN_PLATFORM_MONITOR_GL

#include "./Human/include/Rendering/API/OPmonitorAPI.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"

void OPmonitorAPIGLInit(OPmonitorAPI* monitor);

struct OPmonitorGL {
	GLFWmonitor* Handle;
};

#endif