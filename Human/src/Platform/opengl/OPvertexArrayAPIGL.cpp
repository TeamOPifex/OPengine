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

	//ui32 i = 0;
	//for (; i < vertexLayout->count; i++)
	//{
	//	OPshaderAttribute shaderAttribute = vertexLayout->attributes[i];
	//	if (shaderAttribute.Location < 0) continue;
	//	//OPshaderAttributeGL* shaderAttributeGL = (OPshaderAttributeGL*)shaderAttribute.internalPtr;		
	//	//OPGLFN(OPint loc = glGetAttribLocation(effectGL->Handle, shaderAttribute.Name));

	//	OPGLFN(glEnableVertexAttribArray(shaderAttribute.Location));
	//	OPGLFN(glVertexAttribPointer(shaderAttribute.Location, shaderAttribute.Elements, shaderAttribute.Type, GL_FALSE, vertexLayout->stride, (const void*)shaderAttribute.Offset));
	//}

	//OPvertexArrayGLBind(vertexArray);
	//for (ui32 i = 0; i < vertexLayout.count; i++)
	//{
	//	OPshaderAttribute shaderAttribute = vertexLayout.attributes[i];
	//	OPGLFN(glEnableVertexAttribArray(i));
	//	OPGLFN(glVertexAttribPointer(i, shaderAttribute.Elements, shaderAttribute.Type, GL_FALSE, vertexLayout.stride, (const void*)shaderAttribute.Offset));
	//}
	//OPvertexArrayGLUnbind(vertexArray);

	return vertexArray;
}

OPvertexArray* OPvertexArrayGLCreate(OPvertexLayout* vertexLayout) {
	OPvertexArray* vertexBuffer = (OPvertexArray*)OPalloc(sizeof(OPvertexArray));
	return OPvertexArrayGLInit(vertexBuffer, vertexLayout);
}

void OPvertexArrayGLBind(OPvertexArray* vertexArray) {
	OPvertexArrayGL* vertexArrayGL = (OPvertexArrayGL*)vertexArray->internalPtr;
	OPGLFN(glBindVertexArray(vertexArrayGL->Handle));
}

void OPvertexArrayGLDraw(OPvertexArray* vertexArray, OPuint count) {
	OPvertexArrayGL* vertexArrayGL = (OPvertexArrayGL*)vertexArray->internalPtr;
	OPGLFN(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL));
}

void OPvertexArrayGLUnbind(OPvertexArray* ptr) {
	OPGLFN(glBindVertexArray(0));
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
	vertexArray->Draw = OPvertexArrayGLDraw;
	vertexArray->Unbind = OPvertexArrayGLUnbind;
	vertexArray->Destroy = OPvertexArrayGLDestroy;
}