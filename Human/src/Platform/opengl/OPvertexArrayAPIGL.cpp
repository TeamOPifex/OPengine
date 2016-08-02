#include "./Human/include/Platform/opengl/OPvertexArrayAPIGL.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPvertexArray.h"
#include "./Human/include/Rendering/OPvertexLayout.h"
#include "./Core/include/OPmemory.h"

void OPvertexArrayGLBind(OPvertexArray* vertexArray);
void OPvertexArrayGLUnbind(OPvertexArray* ptr);

OPvertexArray* OPvertexArrayGLInit(OPvertexArray* vertexArray, OPvertexLayout* vertexLayout) {
	OPvertexArrayGL* vertexArrayGL = (OPvertexArrayGL*)OPalloc(sizeof(OPvertexArrayGL));
	vertexArray->internalPtr = vertexArrayGL;
	//vertexArray->vertexLayout = vertexLayout;
	OPGLFN(glGenVertexArrays(1, &vertexArrayGL->Handle));

	return vertexArray;
}

OPvertexArray* OPvertexArrayGLCreate(OPvertexLayout* vertexLayout) {
	OPvertexArray* vertexBuffer = (OPvertexArray*)OPalloc(sizeof(OPvertexArray));
	return OPvertexArrayGLInit(vertexBuffer, vertexLayout);
}

void OPvertexArrayGLBind(OPvertexArray* vertexArray) {
	OPvertexArrayGL* vertexArrayGL = (OPvertexArrayGL*)vertexArray->internalPtr;
	OPGLFN(glBindVertexArray(vertexArrayGL->Handle));
	OPRENDERER_ACTIVE->OPVERTEXARRAY_ACTIVE = vertexArray;
}

void OPvertexArrayGLDraw(OPvertexArray* vertexArray, OPuint count, OPuint offset) {
	OPvertexArrayGL* vertexArrayGL = (OPvertexArrayGL*)vertexArray->internalPtr;
	OPGLFN(glDrawArrays(GL_TRIANGLES, offset, (GLsizei)count));
}

void OPvertexArrayGLDrawIndexed(OPvertexArray* vertexArray, OPuint count, OPuint offset) {
	OPvertexArrayGL* vertexArrayGL = (OPvertexArrayGL*)vertexArray->internalPtr;
	OPGLFN(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint))));
}

void OPvertexArrayGLUnbind(OPvertexArray* ptr) {
	OPGLFN(glBindVertexArray(0));
	OPRENDERER_ACTIVE->OPVERTEXARRAY_ACTIVE = NULL;
}

void OPvertexArrayGLDestroy(OPvertexArray* vertexArray) {
	OPvertexArrayGL* vertexArrayGL = (OPvertexArrayGL*)vertexArray->internalPtr;
	OPGLFN(glDeleteVertexArrays(1, &vertexArrayGL->Handle));
	OPfree(vertexArrayGL);
	vertexArray->internalPtr = NULL;
}

void OPvertexArrayAPIGLInit(OPvertexArrayAPI* vertexArray) {
	vertexArray->Init = OPvertexArrayGLInit;
	vertexArray->Create = OPvertexArrayGLCreate;
	vertexArray->Bind = OPvertexArrayGLBind;
	vertexArray->_Draw = OPvertexArrayGLDraw;
	vertexArray->_DrawIndexed = OPvertexArrayGLDrawIndexed;
	vertexArray->Unbind = OPvertexArrayGLUnbind;
	vertexArray->Destroy = OPvertexArrayGLDestroy;
}