#include "GLMaterial.h"
#include <GL/glew.h>

using namespace OPifex;

void OPifex::GLMaterial::load(ShaderPtr vertex, ShaderPtr fragment){
	m_handle = glCreateProgram();
	if(m_handle){
		glAttachShader(m_handle, (GLuint)vertex->handle());
		glAttachShader(m_handle, (GLuint)fragment->handle());
		glLinkProgram(m_handle);
	}
}

ui32 OPifex::GLMaterial::attribute_location(const char* attribute){
	return glGetAttribLocation(m_handle, "vPosition");
}

void OPifex::GLMaterial::set_data(ui32 attribute, ui32 size, bool normalize, ui32 stride, void* arrayData){
	glVertexAttribPointer((GLuint)attribute, size, GL_FLOAT, normalize, stride, arrayData);
}

void OPifex::GLMaterial::enable_attrib(ui32 attribute){
	glEnableVertexAttribArray(attribute);
}

void OPifex::GLMaterial::disable_attrib(ui32 attribute){
	glDisableVertexAttribArray(attribute);
}