#include "GLBuffer.h"
#include "./Human/Rendering/GL/GLUtility.h"

#ifdef OPIFEX_OPENGL_ES_2
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
#endif

void GLBuffer::load(int bufferType, ui32 size, const void* data){	
	
	GLUtility::CheckError("GLBuffer::Clearing Errors");

	glGenBuffers(1, &m_handle);
	GLUtility::CheckError("GLBuffer::1 - ERROR!");
		
	if(bufferType == 1)
		glBindBuffer(GL_ARRAY_BUFFER, m_handle);
	else if(bufferType == 2)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
	GLUtility::CheckError("GLBuffer::2 - ERROR!");
	
	if(bufferType == 1)
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	else if(bufferType == 2)
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	GLUtility::CheckError("GLBuffer::3 - ERROR!");

#ifdef OPIFEX_ANDROID

#else
	//glGenVertexArrays(1, &m_vertex_handle);
	//GLUtility::CheckError("GLBuffer::4 - ERROR!");

	//glBindVertexArray(m_vertex_handle);
	//GLUtility::CheckError("GLBuffer::5 - ERROR!");
#endif

}
