#include "./Human/include/Utilities/Errors.h"
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
		OPLog_i32(err);
		return true;
	}
	return false;
}