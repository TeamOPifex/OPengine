#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "GLESMaterial.h"


void GLESMaterial::load(ShaderPtr vertex, ShaderPtr fragment){
	m_handle = glCreateProgram();
	if(m_handle){
		glAttachShader(m_handle, (GLuint)vertex->handle());
		glAttachShader(m_handle, (GLuint)fragment->handle());
		glLinkProgram(m_handle);
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