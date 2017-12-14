#include "./Human/include/Platform/opengles/OPcommonGLES.h"

#ifdef OPIFEX_OPENGL_ES

#include "./Core/include/OPlog.h"

GLenum OPcommonGLCheckError() {
    return 0;
}

//void GetFirstNMessages()
//{
//	GLint maxMsgLen = 0;
//	glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &maxMsgLen);
//
//	GLchar msgData[255 * 10];
//	GLenum sources[10];
//	GLenum types[10];
//	GLenum severities[10];
//	GLuint ids[10];
//	GLsizei lengths[10];
//
//	GLuint numFound = glGetDebugMessageLog(10, 255, &sources[0], &types[0], &ids[0], &severities[0], &lengths[0], &msgData[0]);
//
//	for (OPuint i = 0; i < numFound; i++) {
//		OPlog((OPchar*)msgData[i]);
//	}
//}

bool OPcommonGLESLog(const OPchar* function, const OPchar* file, i32 line) {
	GLenum error = OPcommonGLCheckError();
	if (error != GL_NO_ERROR)
	{
		//GetFirstNMessages();
		OPlogErr("OpenGL Error: [%d] %s, %s:%d", error, function, file, line);
		return false;
	}
	return true;
}

#endif
