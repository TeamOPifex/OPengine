#include "./Human/include/Platform/opengl/OPvertexBufferAPIGL.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Core/include/OPmemory.h"

OPvertexBuffer* OPvertexBufferGLInit(OPvertexBuffer* vertexBuffer) {
	OPvertexBufferAPIGL* internalPtr = (OPvertexBufferAPIGL*)OPalloc(sizeof(OPvertexBufferAPIGL));;
	vertexBuffer->internalPointer = internalPtr;
	OPGLFN(glGenBuffers(1, &internalPtr->Handle));
	return vertexBuffer;
}

OPvertexBuffer* OPvertexBufferGLCreate() {
	OPvertexBuffer* vertexBuffer = (OPvertexBuffer*)OPalloc(sizeof(OPvertexBuffer));
	return OPvertexBufferGLInit(vertexBuffer);
}

void OPvertexBufferGLSetData(OPvertexBuffer* vertexBuffer, ui32 elementSize, OPuint count, const void* data) {
	OPvertexBufferAPIGL* buffer = (OPvertexBufferAPIGL*)vertexBuffer->internalPointer;
	vertexBuffer->ElementSize = elementSize;
	vertexBuffer->ElementCount = count;
	OPGLFN(glBufferData(GL_ARRAY_BUFFER, elementSize * count, data, GL_STATIC_DRAW));
}

void OPvertexBufferGLBind(OPvertexBuffer* vertexBuffer) {
	OPvertexBufferAPIGL* buffer = (OPvertexBufferAPIGL*)vertexBuffer->internalPointer;
	OPGLFN(glBindBuffer(GL_ARRAY_BUFFER, buffer->Handle));
}

void OPvertexBufferGLUnbind(OPvertexBuffer* vertexBuffer) {
	OPvertexBufferAPIGL* buffer = (OPvertexBufferAPIGL*)vertexBuffer->internalPointer;
}

void OPvertexBufferGLDestroy(OPvertexBuffer* vertexBuffer) {
	OPvertexBufferAPIGL* buffer = (OPvertexBufferAPIGL*)vertexBuffer->internalPointer;
	OPGLFN(glDeleteBuffers(1, &buffer->Handle));
}

void OPvertexBufferAPIGLInit(OPvertexBufferAPI* vertexBuffer) {
	vertexBuffer->Init = OPvertexBufferGLInit;
	vertexBuffer->Create = OPvertexBufferGLCreate;
	vertexBuffer->SetData = OPvertexBufferGLSetData;
	vertexBuffer->Bind = OPvertexBufferGLBind;
	vertexBuffer->Unbind = OPvertexBufferGLUnbind;
	vertexBuffer->Destroy = OPvertexBufferGLDestroy;
}