#include "GLESBuffer.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

void GLESBuffer::load(int shaderType, ui32 size, const f32* data){	
	glGenBuffers(1, &m_handle);	
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	//glGenVertexArrays(1, &m_vertex_handle);
	//glBindVertexArray(m_vertex_handle);
}