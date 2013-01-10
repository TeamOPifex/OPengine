#include "GLUtility.h"

#ifdef OPIFEX_OPENGL_ES_2
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
#endif

static GLenum err;
bool GLUtility::CheckError(const char* message){
	err = glGetError();
	if(err != 0){
		OPLog(message);
		return true;
	}
	return false;
}