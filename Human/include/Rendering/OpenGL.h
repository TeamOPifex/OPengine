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
