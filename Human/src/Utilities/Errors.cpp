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
		OPLog(message);
		OPLog_i32(err);
		return 1;
	}
	return 0;
}