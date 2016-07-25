#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Core/include/OPlog.h"

GLenum OPcommonGLCheckError() {
	return glGetError();
}

bool OPcommonGLLog(const OPchar* function, const OPchar* file, i32 line) {
	GLenum error = OPcommonGLCheckError();
	if (error != GL_NO_ERROR)
	{
		OPlogErr("OpenGL Error: [%d] %s, %s, %s", error, function, file, line);
		return false;
	}
	return true;
}