#include "./Human/include/Utilities/Errors.h"

#ifdef OPIFEX_OPENGL_ES_2
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
#endif

OPint CheckError(const char* message){
	GLenum err = glGetError();
	if(err != 0){
		OPlog(message);

		switch(err){
			case GL_INVALID_ENUM:
				OPlog("GL_INVALID_ENUM");
				break;
			case GL_INVALID_VALUE:
				OPlog("GL_INVALID_VALUE");
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				OPlog("GL_INVALID_FRAMEBUFFER_OPERATION");
				break;
			case GL_OUT_OF_MEMORY:
				OPlog("GL_OUT_OF_MEMORY");
				break;
			case GL_STACK_UNDERFLOW:
				OPlog("GL_STACK_UNDERFLOW");
				break;
			case GL_STACK_OVERFLOW:
				OPlog("GL_STACK_OVERFLOW");
				break;
		}

		return 1;
	}
	return 0;
}