#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "GLESMaterial.h"
#include "Core\include\Log.h"

void GLESMaterial::load(ShaderPtr vertex, ShaderPtr fragment){
	m_handle = glCreateProgram();
	OPLog("Create Material >");
	OPLogNum(m_handle);
	
	GLenum err;	
	err = glGetError();
	if(err != GL_NO_ERROR){
		OPLog("GLESMaterial::1 - ERROR!");
		return;
	}

	if(m_handle){

		glAttachShader(m_handle, (GLuint)vertex->handle());	
		err = glGetError();
		if(err != GL_NO_ERROR){
			OPLog("GLESMaterial::2 - ERROR!");
			OPLogNum(vertex->handle());
			return;
		}

		glAttachShader(m_handle, (GLuint)fragment->handle());	
		err = glGetError();
		if(err != GL_NO_ERROR){
			OPLog("GLESMaterial::3 - ERROR!");
			OPLogNum(fragment->handle());
			return;
		}

		glLinkProgram(m_handle);
		err = glGetError();
		if(err != GL_NO_ERROR){
			OPLog("GLESMaterial::4 - ERROR!");
			return;
		}

	}
}

ui32 GLESMaterial::attribute_location(const char* attribute){
	return glGetAttribLocation(m_handle, attribute);
}

ui32 GLESMaterial::uniform_location(const char* attribute){
	return glGetUniformLocation(m_handle, attribute);
}

void GLESMaterial::set_data(ui32 attribute, ui32 size, bool normalize, ui32 stride, void* arrayData){
	glVertexAttribPointer((GLuint)attribute, size, GL_FLOAT, normalize, stride, arrayData);
}

void GLESMaterial::enable_attrib(ui32 attribute){
	glEnableVertexAttribArray(attribute);
}

void GLESMaterial::disable_attrib(ui32 attribute){
	glDisableVertexAttribArray(attribute);
}

void GLESMaterial::set_matrix(ui32 attribute, f32* matrix){
	glUniformMatrix4fv(attribute, 1, GL_FALSE, matrix);
}