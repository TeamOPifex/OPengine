#include <GL/glew.h>
#include "GLShader.h"
#include "GLTypes.h"

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
	if(ptr){
		glShaderSource(ptr, 1, &file, 0);
		glCompileShader(ptr);
		GLint compiled = 0;
		glGetShaderiv(ptr, GL_COMPILE_STATUS, &compiled);
		if(!compiled){
			// Throw Error			
			glDeleteShader(ptr);
		}
	}
}