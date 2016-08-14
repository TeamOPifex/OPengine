#pragma once

#include "./Core/include/OPtypes.h"

#ifdef OPIFEX_OPENGL_ES_2
	#ifdef OPIFEX_IOS
		#include <OpenGLES/ES2/gl.h>
	#else
		#include <GLES2/gl2.h>
		#include <GLES2/gl2ext.h>
	#endif
#else
	#include <GL/glew.h>
	#include <GLFW/glfw3.h>
#endif

#ifndef OPIFEX_WINDOWS
	#define __debugbreak __builtin_trap
#endif

i8 OPglewInit();
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