#include "Shader.h"
#include "./Human/Utilities/Errors.h"
#include "./Human/Resources/Material/Shader/ShaderTypes.h"

#ifdef OPIFEX_OPENGL_ES_2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

Shader::Shader(ShaderType shaderType, const char* file){
	m_handle = glCreateShader(GetShaderType(shaderType));
	CheckError("GLShader::Error 1");
	if(m_handle){
		glShaderSource(m_handle, 1, &file, 0);
		CheckError("GLShader::Error 2");
		glCompileShader(m_handle);
		CheckError("GLShader::Error 3");
		GLint compiled = 0;
		glGetShaderiv(m_handle, GL_COMPILE_STATUS, &compiled);
		CheckError("GLShader::Error 4");
		if(!compiled){
			CheckError("GLShader::Error 5");
			glDeleteShader(m_handle);
		}
	}
}