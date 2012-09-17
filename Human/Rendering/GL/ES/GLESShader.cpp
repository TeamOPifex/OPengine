#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "GLESShader.h"
#include "GLESTypes.h"

GLESShader::GLESShader(){
}

GLESShader::GLESShader(ShaderType shaderType, const char* file){
	load(shaderType, file);
}

GLESShader::~GLESShader(){
	if(ptr != 0){
		glDeleteShader(ptr);
	}
}

void GLESShader::load(ShaderTypes::ShaderType shaderType, const char* file){
	ptr = glCreateShader(ShaderTypes::gl_shader_type(shaderType));
	if(ptr){
		glShaderSource(ptr, 1, &file, 0);
		glCompileShader(ptr);
		GLint compiled = 0;
		glGetShaderiv(ptr, GL_COMPILE_STATUS, &compiled);
		if(!compiled){
			// Throw Error			
			glDeleteShader(ptr);
			ptr = 0;
		}
	}
}
