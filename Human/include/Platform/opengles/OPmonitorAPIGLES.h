#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL_ES

struct OPmonitorGLES;
typedef struct OPmonitorGLES OPmonitorGLES;

#include "./Human/include/Rendering/API/OPmonitorAPI.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"

void OPmonitorAPIGLESInit(OPmonitorAPI* monitor);

struct OPmonitorGLES {
#ifndef OPIFEX_ANDROID
	GLFWmonitor* Handle;
#endif
};

#endif