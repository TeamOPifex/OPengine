#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "GLESShader.h"
#include "GLESTypes.h"
#include "Core\include\Log.h"

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

void GLESShader::load(ShaderType shaderType, const char* file){
	
	OPLog("Create Shader >");

	ptr = glCreateShader(gl_shader_type(shaderType));
	
	GLenum err;	
	err = glGetError();
	if(err != GL_NO_ERROR){
		OPLog("GLESShader::1 - ERROR!");
		return;
	}

	if(ptr){

		glShaderSource(ptr, 1, &file, 0);
		err = glGetError();
		if(err != GL_NO_ERROR){
			OPLog("GLESShader::2 - ERROR!");
			return;
		}

		glCompileShader(ptr);
		err = glGetError();
		if(err != GL_NO_ERROR){
			OPLog("GLESShader::3 - ERROR!");
			return;
		}

		GLint compiled = 0;
		glGetShaderiv(ptr, GL_COMPILE_STATUS, &compiled);
		err = glGetError();
		if(err != GL_NO_ERROR){
			OPLog("GLESShader::4 - ERROR!");
			return;
		}

		if(!compiled){
			OPLog("GLESShader::5 - ERROR!");
			// Throw Error			
			glDeleteShader(ptr);
			ptr = 0;
			return;
		}
	} else {
		OPLog("GLESShader::6 - ERROR!");
		return;
	}
	
	
	OPLog("Shader Created");
	OPLogNum(ptr);
}
