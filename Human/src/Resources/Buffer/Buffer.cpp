#include "./Human/include/Resources/Buffer/Buffer.h"
#include "./Human/include/Utilities/Errors.h"

#ifdef OPIFEX_OPENGL_ES_2
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
#endif

Buffer::Buffer(BufferType shaderType, ui32 size, const void* data){	
	glGenBuffers(1, &m_handle);
	CheckError("Buffer::1 - ERROR!");
		
	if(shaderType == VertexBuffer)
		glBindBuffer(GL_ARRAY_BUFFER, m_handle);
	else if(shaderType == IndexBuffer)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
	CheckError("Buffer::2 - ERROR!");
	
	if(shaderType == VertexBuffer)
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	else if(shaderType == IndexBuffer)
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	CheckError("Buffer::3 - ERROR!");

#ifdef OPIFEX_ANDROID

#else
	//glGenVertexArrays(1, &m_vertex_handle);
	//GLUtility::CheckError("GLBuffer::4 - ERROR!");

	//glBindVertexArray(m_vertex_handle);
	//GLUtility::CheckError("GLBuffer::5 - ERROR!");
#endif
}