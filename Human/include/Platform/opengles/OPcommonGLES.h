#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL_ES
	#ifdef OPIFEX_IOS
		#include <OpenGLES/ES2/gl.h>
	#endif
	//#ifdef OPIFEX_ANDROID
		#include <EGL/egl.h> 
		#include <GLES/gl.h>
		#include <GLES2/gl2.h>
		#include <GLES2/gl2ext.h>
	//#endif

	GLenum OPcommonGLESCheckError();
	bool OPcommonGLESLog(const OPchar* function, const OPchar* file, i32 line);

	//OPlog("%s, %s, %d", #x, __FILE__, __LINE__);

	#ifdef _DEBUG
	#define OPGLFN(x) OPcommonGLESCheckError();\
			x; \
			if (!OPcommonGLESLog(#x, __FILE__, __LINE__)) { \
				OPlogErr("GLERROR"); \
			}
	#else
		#define OPGLFN(x) x
	#endif

#endif