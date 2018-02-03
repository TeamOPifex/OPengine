#include "./Human/include/Platform/opengl/OPcommonGL.h"

#ifdef OPIFEX_OPENGL

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

bool OPcommonGLLog(const OPchar* function, const OPchar* file, i32 line) {
	GLenum error = OPcommonGLCheckError();
	if (error != GL_NO_ERROR)
	{
		//GetFirstNMessages();
		OPlogErr("OpenGL Error: [%d] %s, %s:%d", error, function, file, line);
		return false;
	}
	return true;
}

OPshaderUniformType::Enum UniformTypeToOPshaderUniformType(GLint type)
{
	const OPchar* s;
	switch (type)
	{
	case GL_FLOAT: return OPshaderUniformType::FLOAT;
	case GL_FLOAT_VEC2: return OPshaderUniformType::VECTOR2;
	case GL_FLOAT_VEC3: return OPshaderUniformType::VECTOR3;
	case GL_FLOAT_VEC4: return OPshaderUniformType::VECTOR4;
	case GL_INT: return OPshaderUniformType::INT;
	case GL_INT_VEC2: return OPshaderUniformType::VECTOR2I;
	case GL_INT_VEC3: return OPshaderUniformType::VECTOR3I;
	case GL_INT_VEC4: return OPshaderUniformType::VECTOR4I;
	case GL_UNSIGNED_INT: return OPshaderUniformType::UINT;
	case GL_UNSIGNED_INT_VEC2: return OPshaderUniformType::VECTOR2UI;
	case GL_UNSIGNED_INT_VEC3: return OPshaderUniformType::VECTOR3UI;
	case GL_UNSIGNED_INT_VEC4: return OPshaderUniformType::VECTOR4UI;
	case GL_BOOL: return OPshaderUniformType::BOOL;
	case GL_BOOL_VEC2: return OPshaderUniformType::VECTOR2B;
	case GL_BOOL_VEC3: return OPshaderUniformType::VECTOR3B;
	case GL_BOOL_VEC4: return OPshaderUniformType::VECTOR4B;
	case GL_FLOAT_MAT2: return OPshaderUniformType::MATRIX2;
	case GL_FLOAT_MAT3: return OPshaderUniformType::MATRIX3;
	case GL_FLOAT_MAT4: return OPshaderUniformType::MATRIX4;
	case GL_FLOAT_MAT2x3: return OPshaderUniformType::MATRIX2X3;
	case GL_FLOAT_MAT2x4: return OPshaderUniformType::MATRIX2X4;
	case GL_FLOAT_MAT3x2: return OPshaderUniformType::MATRIX3X2;
	case GL_FLOAT_MAT3x4: return OPshaderUniformType::MATRIX3X4;
	case GL_FLOAT_MAT4x2: return OPshaderUniformType::MATRIX4X2;
	case GL_FLOAT_MAT4x3: return OPshaderUniformType::MATRIX4X3;
	case GL_SAMPLER_1D: return OPshaderUniformType::TEXTURE;
	case GL_SAMPLER_2D: return OPshaderUniformType::TEXTURE;
	case GL_SAMPLER_3D: return OPshaderUniformType::TEXTURE;
	case GL_SAMPLER_CUBE: return OPshaderUniformType::TEXTURE_CUBE;
	default: return OPshaderUniformType::UNKNOWN;
	}
	return OPshaderUniformType::UNKNOWN;
}

#endif