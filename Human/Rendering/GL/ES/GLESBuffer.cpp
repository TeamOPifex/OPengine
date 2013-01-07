#include "GLESBuffer.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "Core/include/Log.h"

void GLESBuffer::load(int shaderType, ui32 size, const f32* data){	

	GLenum err;

	glGenBuffers(1, &m_handle);
	err = glGetError();
	if(err != GL_NO_ERROR){
		OPLog("GLESBuffer::1 - ERROR!");
		// try once more...
		
		glGenBuffers(1, &m_handle);
		err = glGetError();
		if(err != GL_NO_ERROR){
			OPLog("GLESBuffer::1.1 - Yep Still an error...");
			return;
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);
	err = glGetError();
	if(err != GL_NO_ERROR){
		OPLog("GLESBuffer::2 - ERROR!");
		return;
	}
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	err = glGetError();
	if(err != GL_NO_ERROR){
		OPLog("GLESBuffer::3 - ERROR!");
		return;
	}

	OPLog("Created Buffer");

	//glGenVertexArrays(1, &m_vertex_handle);
	//glBindVertexArray(m_vertex_handle);
}