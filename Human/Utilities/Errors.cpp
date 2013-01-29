#include "Errors.h"
#include "Core/include/Log.h"

#ifdef OPIFEX_OPENGL_ES_2
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
#endif

bool CheckError(const char* message){
	GLenum err = glGetError();
	if(err != 0){
		OPLog(message);
		return true;
	}
	return false;
}