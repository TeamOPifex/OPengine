
#include "./Human/include/Platform/opengles/OPindexBufferGLES.h"

#ifdef OPIFEX_OPENGL_ES_2
#include "./Human/include/Rendering/OPindexBuffer.h"
#include "./Human/include/Platform/opengles/OPcommonGLES.h"
#include "./Core/include/OPmemory.h"

OPindexBuffer* OPindexBufferGLESInit(OPindexBuffer* indexBuffer) {
	OPindexBufferAPIGLES* internalPtr = (OPindexBufferAPIGLES*)OPalloc(sizeof(OPindexBufferAPIGLES));;
	indexBuffer->internalPointer = internalPtr;
	OPGLFN(glGenBuffers(1, &internalPtr->Handle));
	return indexBuffer;
}

OPindexBuffer* OPindexBufferGLESCreate() {
	OPindexBuffer* indexBuffer = (OPindexBuffer*)OPalloc(sizeof(OPindexBuffer));
	return OPindexBufferGLESInit(indexBuffer);
}

void OPindexBufferGLESSetData(OPindexBuffer* indexBuffer, OPindexSize::Enum size, ui32 count, const void* data) {
	OPindexBufferAPIGLES* buffer = (OPindexBufferAPIGLES*)indexBuffer->internalPointer;
	OPGLFN(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->Handle));
	indexBuffer->ElementSize = size;
	indexBuffer->ElementCount = count;
	ui32 totalSize = (ui32)indexBuffer->ElementSize * count;
	OPGLFN(glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalSize, data, GL_STATIC_DRAW));
}

void OPindexBufferGLESBind(OPindexBuffer* indexBuffer) {
	OPindexBufferAPIGLES* buffer = (OPindexBufferAPIGLES*)indexBuffer->internalPointer;
	OPGLFN(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->Handle));
	OPRENDERER_ACTIVE->OPINDEXBUFFER_ACTIVE = indexBuffer;
}

void OPindexBufferGLESUnbind(OPindexBuffer* indexBuffer) {
	OPindexBufferAPIGLES* buffer = (OPindexBufferAPIGLES*)indexBuffer->internalPointer;
	OPGLFN(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	OPRENDERER_ACTIVE->OPINDEXBUFFER_ACTIVE = NULL;
}

void OPindexBufferGLESDestroy(OPindexBuffer* indexBuffer) {
	OPindexBufferAPIGLES* buffer = (OPindexBufferAPIGLES*)indexBuffer->internalPointer;
	OPindexBufferGLESUnbind(indexBuffer);
	OPGLFN(glDeleteBuffers(1, &buffer->Handle));
	OPfree(buffer);
	indexBuffer->internalPointer = NULL;
}

void OPindexBufferAPIGLESInit(OPindexBufferAPI* indexBuffer) {
	indexBuffer->Init = OPindexBufferGLESInit;
	indexBuffer->Create = OPindexBufferGLESCreate;
	indexBuffer->SetData = OPindexBufferGLESSetData;
	indexBuffer->Bind = OPindexBufferGLESBind;
	indexBuffer->Unbind = OPindexBufferGLESUnbind;
	indexBuffer->Destroy = OPindexBufferGLESDestroy;
}

#endif