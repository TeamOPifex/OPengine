#include "GLBuffer.h"
#include <GL/glew.h>

using namespace OPifex;

void OPifex::GLBuffer::load(BufferType shaderType, ui32 size, const f32* data){	
	glGenBuffers(1, &m_handle);	
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glGenVertexArrays(1, &m_vertex_handle);
	glBindVertexArray(m_vertex_handle);
}

void GLBuffer::enable_attribute(ui32 attribute){
	glEnableVertexAttribArray(attribute);
}

void GLBuffer::disable_attribute(ui32 attribute){
	glDisableVertexAttribArray(attribute);
}