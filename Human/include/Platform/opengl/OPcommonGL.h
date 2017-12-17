#pragma once

#include "./Core/include/OPtypes.h"
#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#ifndef OPIFEX_WINDOWS
	#define __debugbreak __builtin_trap
#endif

bool OPglewInit();
GLenum OPcommonGLCheckError();
bool OPcommonGLLog(const OPchar* function, const OPchar* file, i32 line);

//OPlog("%s, %s, %d", #x, __FILE__, __LINE__);

#ifdef _DEBUG
#define OPGLFN(x) OPcommonGLCheckError();\
		x; \
		if (!OPcommonGLLog(#x, __FILE__, __LINE__)) { \
            __debugbreak(); \
        }
#else
	#define OPGLFN(x) x
#endif

#endif