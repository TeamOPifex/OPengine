#include "GLBuffer.h"
#include "Human\Rendering\GL\GLUtility.h"

#ifdef OPIFEX_OPENGL_ES_2
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
#endif

void GLBuffer::load(int shaderType, ui32 size, const f32* data){	
	
#ifdef OPIFEX_ANDROID
	glGenBuffers(1, &m_handle);
	GLUtility::CheckError("GLBuffer::1 - ERROR!");
	
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);
	GLUtility::CheckError("GLBuffer::2 - ERROR!");

	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	GLUtility::CheckError("GLBuffer::3 - ERROR!");
#else
	glGenBuffers(1, &m_handle);	
	GLUtility::CheckError("GLBuffer::1 - ERROR!");

	glBindBuffer(GL_ARRAY_BUFFER, m_handle);
	GLUtility::CheckError("GLBuffer::2 - ERROR!");

	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	GLUtility::CheckError("GLBuffer::3 - ERROR!");

	glGenVertexArrays(1, &m_vertex_handle);
	GLUtility::CheckError("GLBuffer::4 - ERROR!");

	glBindVertexArray(m_vertex_handle);
	GLUtility::CheckError("GLBuffer::5 - ERROR!");
#endif

}