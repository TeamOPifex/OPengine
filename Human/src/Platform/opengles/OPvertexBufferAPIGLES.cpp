
#include "./Human/include/Platform/opengles/OPvertexBufferAPIGLES.h"

#ifdef OPIFEX_OPENGL_ES_2

#include "./Human/include/Platform/opengles/OPeffectAPIGLES.h"
#include "./Human/include/Platform/opengles/OPshaderAttributeAPIGLES.h"
#include "./Human/include/Platform/opengles/OPcommonGLES.h"
#include "./Human/include/Rendering/OPvertexLayout.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/Enums/OPshaderType.h"
#include "./Core/include/OPmemory.h"


OPvertexBuffer* OPvertexBufferGLESInit(OPvertexBuffer* vertexBuffer) {
	OPvertexBufferGLES* internalPtr = (OPvertexBufferGLES*)OPalloc(sizeof(OPvertexBufferGLES));;
	vertexBuffer->internalPtr = internalPtr;
	OPGLFN(glGenBuffers(1, &internalPtr->Handle));
	return vertexBuffer;
}

OPvertexBuffer* OPvertexBufferGLESCreate() {
	OPvertexBuffer* vertexBuffer = (OPvertexBuffer*)OPalloc(sizeof(OPvertexBuffer));
	return OPvertexBufferGLESInit(vertexBuffer);
}

void OPvertexBufferGLESSetData(OPvertexBuffer* vertexBuffer, ui32 elementSize, OPuint count, const void* data) {
	OPvertexBufferGLES* buffer = (OPvertexBufferGLES*)vertexBuffer->internalPtr;
	OPGLFN(glBindBuffer(GL_ARRAY_BUFFER, buffer->Handle));
	vertexBuffer->ElementSize = elementSize;
	vertexBuffer->ElementCount = count;
	OPGLFN(glBufferData(GL_ARRAY_BUFFER, elementSize * count, data, GL_STATIC_DRAW));
}

void OPvertexBufferGLESBind(OPvertexBuffer* vertexBuffer) {
	OPvertexBufferGLES* buffer = (OPvertexBufferGLES*)vertexBuffer->internalPtr;
	OPGLFN(glBindBuffer(GL_ARRAY_BUFFER, buffer->Handle));
	OPRENDERER_ACTIVE->OPVERTEXBUFFER_ACTIVE = vertexBuffer;
}

void OPvertexBufferGLESUnbind(OPvertexBuffer* vertexBuffer) {
	OPvertexBufferGLES* buffer = (OPvertexBufferGLES*)vertexBuffer->internalPtr;
	OPGLFN(glBindBuffer(GL_ARRAY_BUFFER, 0));
	OPRENDERER_ACTIVE->OPVERTEXBUFFER_ACTIVE = NULL;
}

void OPvertexBufferGLESDestroy(OPvertexBuffer* vertexBuffer) {
	OPvertexBufferGLES* buffer = (OPvertexBufferGLES*)vertexBuffer->internalPtr;
	OPvertexBufferGLESUnbind(vertexBuffer);
	OPGLFN(glDeleteBuffers(1, &buffer->Handle));
	OPfree(buffer);
	vertexBuffer->internalPtr = NULL;
}

void OPvertexBufferAPIGLESInit(OPvertexBufferAPI* vertexBuffer) {
	vertexBuffer->Init = OPvertexBufferGLESInit;
	vertexBuffer->Create = OPvertexBufferGLESCreate;
	vertexBuffer->SetData = OPvertexBufferGLESSetData;
	vertexBuffer->Bind = OPvertexBufferGLESBind;
	vertexBuffer->Unbind = OPvertexBufferGLESUnbind;
	vertexBuffer->Destroy = OPvertexBufferGLESDestroy;
}

#endif