#include "Shader.h"
#include "./Human/Utilities/Errors.h"
#include "./Human/Resources/Material/Shader/ShaderTypes.h"
#include "./Core/include/DynamicMemory.h"
#include "./Data/include/OPfile.h"
#include "./Core/include/Log.h"

#ifdef OPIFEX_OPENGL_ES_2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

Shader::Shader(ShaderType shaderType, const char* source){
	
	
	m_handle = glCreateShader(GetShaderType(shaderType));
	CheckError("GLShader::Error 1");
	if(m_handle){
		glShaderSource(m_handle, 1, &source, 0);
		CheckError("GLShader::Error 2");
		glCompileShader(m_handle);
		CheckError("GLShader::Error 3");
		GLint compiled = 0;
		glGetShaderiv(m_handle, GL_COMPILE_STATUS, &compiled);
		CheckError("GLShader::Error 4");
		if(!compiled){
			OPLog("GLShader::Failed to compile Shader");

			char msg[4096];
			i32 length = 0;
			glGetShaderInfoLog(m_handle, 4096, &length, msg);
			OPLog(msg);

			CheckError("GLShader::Error 5");
			glDeleteShader(m_handle);
		}
	}
}

ShaderPtr Shader::FromFile(ShaderType shaderType, const char* file){
	FileInformation fileInfo = OPreadFileInformation(file);
	char* shaderCode = (char*)OPalloc(sizeof(char) * fileInfo.length);
	fgets(shaderCode, fileInfo.length, fileInfo.file);
	shaderCode[fileInfo.length - 1] = '\0';
	ShaderPtr glshader = new Shader(shaderType, shaderCode);
	OPfree(shaderCode);
	return glshader;
}