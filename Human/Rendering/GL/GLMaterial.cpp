#include "GLMaterial.h"
#include "./Human/Rendering/GL/GLUtility.h"

#ifdef OPIFEX_OPENGL_ES_2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

void GLMaterial::load(ShaderPtr vertex, ShaderPtr fragment){
	m_handle = glCreateProgram();
	GLUtility::CheckError("GLMaterial::Error 1");

	if(m_handle){
		glAttachShader(m_handle, (GLuint)vertex->handle());
		GLUtility::CheckError("GLMaterial::Error 2");

		glAttachShader(m_handle, (GLuint)fragment->handle());
		GLUtility::CheckError("GLMaterial::Error 3");

		glLinkProgram(m_handle);
		GLUtility::CheckError("GLMaterial::Error 4");
	}
}

ui32 GLMaterial::attribute_location(const char* attribute){
	return glGetAttribLocation(m_handle, attribute);
}

ui32 GLMaterial::uniform_location(const char* attribute){
	return glGetUniformLocation(m_handle, attribute);
}

void GLMaterial::set_data(ui32 attribute, ui32 size, bool normalize, ui32 stride, void* arrayData){
	glVertexAttribPointer((GLuint)attribute, size, GL_FLOAT, normalize, stride, arrayData);
}

void GLMaterial::enable_attrib(ui32 attribute){
	glEnableVertexAttribArray(attribute);
}

void GLMaterial::disable_attrib(ui32 attribute){
	glDisableVertexAttribArray(attribute);
}

void GLMaterial::set_matrix(ui32 attribute, f32* matrix){
	glUniformMatrix4fv(attribute, 1, GL_FALSE, matrix);
}
