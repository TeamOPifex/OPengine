#pragma once

struct OPmonitorGL;
typedef struct OPmonitorGL OPmonitorGL;

#include "./Human/include/Rendering/API/OPmonitorAPI.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"

void OPmonitorAPIGLInit(OPmonitorAPI* monitor);

struct OPmonitorGL {
#ifndef OPIFEX_ANDROID
	GLFWmonitor* Handle;
#endif
};