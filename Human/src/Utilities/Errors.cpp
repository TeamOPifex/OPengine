#include "./Human/include/Utilities/Errors.h"
#include "./Human/include/Rendering/OPrender.h"
#include "Core/include/OPlog.h"
#include "Core/include/Assert.h"

#ifdef OPIFEX_OPENGL_ES_2
    #ifdef OPIFEX_IOS
    #include <OpenGLES/ES2/gl.h>
    #else
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    #endif
#else
	#include <GL/glew.h>
#endif

OPint OPglError(const char* message){
	ASSERT(OPRENDER_INITIALIZED != 0, "OPrenderInit must be called first.");

	GLenum err = glGetError();
	if(err != 0){
		OPlogErr(message, err);

		const OPchar* errMsg = "OPENGLERROR";// (OPchar*)gluErrorString(err);
		OPlogErr(errMsg);

		switch(err){
			case GL_INVALID_ENUM:
				OPlogErr("GL_INVALID_ENUM");
				break;
			case GL_INVALID_OPERATION:
				OPlogErr("GL_INVALID_OPERATION");
				break;
			case GL_INVALID_VALUE:
				OPlogErr("GL_INVALID_VALUE");
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				OPlogErr("GL_INVALID_FRAMEBUFFER_OPERATION");
				break;
			case GL_OUT_OF_MEMORY:
				OPlogErr("GL_OUT_OF_MEMORY");
				break;
			#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS)
				case GL_STACK_UNDERFLOW:
					OPlogErr("GL_STACK_UNDERFLOW");
					break;
				case GL_STACK_OVERFLOW:
					OPlogErr("GL_STACK_OVERFLOW");
					break;
			#endif
		}

		return 1;
	}
	return 0;
}
