#include "GLShader.h"
#include "GLTypes.h"
#include "./Human/Rendering/GL/GLUtility.h"

#ifdef OPIFEX_OPENGL_ES_2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

GLShader::GLShader(){
}

GLShader::GLShader(ShaderType shaderType, const char* file){
	load(shaderType, file);
}

GLShader::~GLShader(){
	if(ptr != 0){
		glDeleteShader(ptr);
	}
}

void GLShader::load(ShaderType shaderType, const char* file){
	ptr = glCreateShader(gl_shader_type(shaderType));
	GLUtility::CheckError("GLShader::Error 1");
	if(ptr){
		glShaderSource(ptr, 1, &file, 0);
		GLUtility::CheckError("GLShader::Error 2");
		glCompileShader(ptr);
		GLUtility::CheckError("GLShader::Error 3");
		GLint compiled = 0;
		glGetShaderiv(ptr, GL_COMPILE_STATUS, &compiled);
		GLUtility::CheckError("GLShader::Error 4");
		if(!compiled){
			GLUtility::CheckError("GLShader::Error 5");
			glDeleteShader(ptr);
		}
	}
}
