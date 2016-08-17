#include "./Human/include/Rendering/OPrenderBuffer.h"
#include "./Human/include/Rendering/Enums/OPattributes.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/Assert.h"

//#ifdef OPIFEX_OPENGL_ES_2
//    #ifdef OPIFEX_IOS
//    #include <OpenGLES/ES2/gl.h>
//    #else
//    #include <GLES2/gl2.h>
//    #include <GLES2/gl2ext.h>
//    #endif
//#else
//    #include <GLFW/glfw3.h>
//    #include <GL/glew.h>
//#endif

void OPrenderBindBuffer(OPvertexBuffer* buffer) {
	buffer->Bind();
}

void OPrenderBindBuffer(OPindexBuffer* buffer) {
	buffer->Bind();
}

//-----------------------------------------------------------------------------
void OPrenderDrawBuffer(ui32 offset) {
	OPRENDERER_ACTIVE->VertexArray.Draw(OPRENDERER_ACTIVE->OPVERTEXARRAY_ACTIVE, OPRENDERER_ACTIVE->OPVERTEXBUFFER_ACTIVE->ElementCount, offset);
}

#include "./Human/include/Rendering/OPrender.h"
//-----------------------------------------------------------------------------
void OPrenderDrawBufferIndexed(ui32 offset){
	OPRENDERER_ACTIVE->VertexArray.DrawIndexed(OPRENDERER_ACTIVE->OPVERTEXARRAY_ACTIVE, OPRENDERER_ACTIVE->OPINDEXBUFFER_ACTIVE->ElementCount);

	//GLenum indType = OPRENDER_CURR_IB->ElementSize == OPindexSize::SHORT ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
	//ASSERT(OPRENDER_CURR_IB != NULL, "Index Buffer hasn't been set");
	//glDrawElements(GL_TRIANGLES, (GLsizei)OPRENDER_CURR_IB->ElementCount, indType, (void*)(offset * sizeof(GLuint)));
}

//-----------------------------------------------------------------------------
void OPrenderDrawIndexed(ui32 offset, ui32 count){
	OPRENDERER_ACTIVE->VertexArray.Draw(OPRENDERER_ACTIVE->OPVERTEXARRAY_ACTIVE, count);
	//GLenum indType = OPRENDER_CURR_IB->ElementSize == OPindexSize::SHORT ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
	//glDrawElements(GL_TRIANGLES, count, indType, (void*)(offset * sizeof(GLuint)));
}

void OPrenderDrawUserArray(void* vertices, ui32 attrs, ui32 offset, ui32 count) {

#if !defined(OPIFEX_ANDROID) || !defined(OPIFEX_IOS)
	OPlog("OPrenderDrawUserArray NOT SUPPORTED yet on Android");
#else
	ui32 off = 0;
	ui32 stride = 0;
	if (attrs & OPATTR_POSITION) stride += 3;
	if (attrs & OPATTR_NORMAL) stride += 3;
	if (attrs & OPATTR_UV) stride += 2;

	if (attrs & OPATTR_POSITION) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, stride, &vertices);
		off += 3;
	}

	if (attrs & OPATTR_NORMAL) {
		glDisableClientState(GL_VERTEX_ARRAY);
		glNormalPointer(GL_FLOAT, stride, &vertices + off * sizeof(f32));
		off += 3;
	}

	if (attrs & OPATTR_UV) {
		glDisableClientState(GL_VERTEX_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, stride, &vertices + off * sizeof(f32));
		off += 2;
	}

	glDrawArrays(GL_TRIANGLES, offset, count);

	if (attrs & OPATTR_POSITION) {
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	if (attrs & OPATTR_NORMAL) {
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	if (attrs & OPATTR_UV) {
		glDisableClientState(GL_VERTEX_ARRAY);
	}
#endif
}
