#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Core/include/OPlog.h"

bool glewInitialized = 0;

bool OPglewInit() {
	if (glewInitialized) return true;
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		OPlogErr("Failed to init glew: %d", result);
		return false;
	}
	glewInitialized = true;
	return true;
}

GLenum OPcommonGLCheckError() {
	return glGetError();
}

bool OPcommonGLLog(const OPchar* function, const OPchar* file, i32 line) {
	GLenum error = OPcommonGLCheckError();
	if (error != GL_NO_ERROR)
	{
		OPlogErr("OpenGL Error: [%d] %s, %s, %d", error, function, file, line);
		return false;
	}
	return true;
}