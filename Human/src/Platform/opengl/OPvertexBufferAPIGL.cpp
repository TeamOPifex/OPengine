#include "./Human/include/Platform/opengl/OPvertexBufferAPIGL.h"
#include "./Human/include/Platform/opengl/OPeffectAPIGL.h"
#include "./Human/include/Platform/opengl/OPshaderAttributeAPIGL.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPvertexLayout.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Core/include/OPmemory.h"

OPvertexBuffer* OPvertexBufferGLInit(OPvertexBuffer* vertexBuffer) {
	OPvertexBufferGL* internalPtr = (OPvertexBufferGL*)OPalloc(sizeof(OPvertexBufferGL));;
	vertexBuffer->internalPtr = internalPtr;
	OPGLFN(glGenBuffers(1, &internalPtr->Handle));
	return vertexBuffer;
}

OPvertexBuffer* OPvertexBufferGLCreate() {
	OPvertexBuffer* vertexBuffer = (OPvertexBuffer*)OPalloc(sizeof(OPvertexBuffer));
	return OPvertexBufferGLInit(vertexBuffer);
}

void OPvertexBufferGLSetData(OPvertexBuffer* vertexBuffer, ui32 elementSize, OPuint count, const void* data) {
	OPvertexBufferGL* buffer = (OPvertexBufferGL*)vertexBuffer->internalPtr;
	OPGLFN(glBindBuffer(GL_ARRAY_BUFFER, buffer->Handle));
	vertexBuffer->ElementSize = elementSize;
	vertexBuffer->ElementCount = count;
	OPGLFN(glBufferData(GL_ARRAY_BUFFER, elementSize * count, data, GL_STATIC_DRAW));
}

void OPvertexBufferGLSetLayout(OPvertexBuffer* vertexBuffer, OPvertexLayout* vertexLayout) {
	ui32 i = 0;
	for (; i < vertexLayout->count; i++)
	{
		OPshaderAttribute shaderAttribute = vertexLayout->attributes[i];
		if (shaderAttribute.Location < 0) continue;
		//OPshaderAttributeGL* shaderAttributeGL = (OPshaderAttributeGL*)shaderAttribute.internalPtr;		
		//OPGLFN(OPint loc = glGetAttribLocation(effectGL->Handle, shaderAttribute.Name));

		OPGLFN(glEnableVertexAttribArray(shaderAttribute.Location));
		OPGLFN(glVertexAttribPointer(shaderAttribute.Location, shaderAttribute.Elements, shaderAttribute.Type, GL_FALSE, vertexLayout->stride, (const void*)shaderAttribute.Offset));
	}
}

void OPvertexBufferGLBind(OPvertexBuffer* vertexBuffer) {
	OPvertexBufferGL* buffer = (OPvertexBufferGL*)vertexBuffer->internalPtr;
	OPGLFN(glBindBuffer(GL_ARRAY_BUFFER, buffer->Handle));
}

void OPvertexBufferGLUnbind(OPvertexBuffer* vertexBuffer) {
	OPvertexBufferGL* buffer = (OPvertexBufferGL*)vertexBuffer->internalPtr;
}

void OPvertexBufferGLDestroy(OPvertexBuffer* vertexBuffer) {
	OPvertexBufferGL* buffer = (OPvertexBufferGL*)vertexBuffer->internalPtr;
	OPvertexBufferGLUnbind(vertexBuffer);
	OPGLFN(glDeleteBuffers(1, &buffer->Handle));
	OPfree(buffer);
	vertexBuffer->internalPtr = NULL;
}

void OPvertexBufferAPIGLInit(OPvertexBufferAPI* vertexBuffer) {
	vertexBuffer->Init = OPvertexBufferGLInit;
	vertexBuffer->Create = OPvertexBufferGLCreate;
	vertexBuffer->SetData = OPvertexBufferGLSetData;
	vertexBuffer->SetLayout = OPvertexBufferGLSetLayout;
	vertexBuffer->Bind = OPvertexBufferGLBind;
	vertexBuffer->Unbind = OPvertexBufferGLUnbind;
	vertexBuffer->Destroy = OPvertexBufferGLDestroy;
}