#include "./Human/include/Platform/opengl/OPvertexArrayAPIGL.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPvertexArray.h"
#include "./Human/include/Rendering/OPvertexLayout.h"
#include "./Core/include/OPmemory.h"

void OPvertexArrayGLBind(OPvertexArray* vertexArray);
void OPvertexArrayGLUnbind(OPvertexArray* ptr);

GLint OPshaderElementTypeToGL(OPshaderElementType shaderElementType) {
	switch (shaderElementType) {
	case OPshaderElementType::FLOAT: return GL_FLOAT;
	case OPshaderElementType::INT: return GL_INT;
	case OPshaderElementType::SHORT: return GL_SHORT;
	}
	return 0;
}

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

#include "./Human/include/Platform/opengl/OPeffectAPIGL.h"
void OPvertexArrayGLSetLayout(OPvertexArray* vertexArray, OPvertexLayout* vertexLayout) {
	OPvertexArrayGLBind(vertexArray);
	
	ui32 i = 0;
	for (; i < vertexLayout->count; i++)
	{
		OPshaderAttribute shaderAttribute = vertexLayout->attributes[i];
		if (shaderAttribute.Location < 0) {
			if (OPRENDERER_ACTIVE->OPEFFECT_ACTIVE != NULL) {
				OPeffectGL* effectGL = (OPeffectGL*)OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->internalPtr;
				OPGLFN(OPint loc = glGetAttribLocation(effectGL->Handle, shaderAttribute.Name));
				shaderAttribute.Location = loc;
			}

			if (shaderAttribute.Location < 0) {
				continue;
			}
		}
		//OPshaderAttributeGL* shaderAttributeGL = (OPshaderAttributeGL*)shaderAttribute.internalPtr;		
		//OPGLFN(OPint loc = glGetAttribLocation(effectGL->Handle, shaderAttribute.Name));

		OPGLFN(glEnableVertexAttribArray((GLuint)shaderAttribute.Location));
		OPGLFN(glVertexAttribPointer((GLuint)shaderAttribute.Location, shaderAttribute.Elements, OPshaderElementTypeToGL(shaderAttribute.Type), GL_FALSE, vertexLayout->stride, (const void*)shaderAttribute.Offset));
	}
}

void OPvertexArrayGLDraw(OPvertexArray* vertexArray, OPuint count, OPuint offset) {
	OPvertexArrayGL* vertexArrayGL = (OPvertexArrayGL*)vertexArray->internalPtr;
	OPGLFN(glDrawArrays(GL_TRIANGLES, (GLint)offset, (GLsizei)count));
}

ui32 OPindexSizeToGL(OPindexSize indexSize) {
	switch (indexSize) {
		case OPindexSize::BYTE: return GL_UNSIGNED_BYTE;
		case OPindexSize::SHORT: return GL_UNSIGNED_SHORT;
		case OPindexSize::INT: return GL_UNSIGNED_INT;
	}
	return 0;
}

#include "./Human/include/Rendering/OPindexBuffer.h"
void OPvertexArrayGLDrawIndexed(OPvertexArray* vertexArray, OPuint count, OPuint offset) {
	OPvertexArrayGL* vertexArrayGL = (OPvertexArrayGL*)vertexArray->internalPtr;	
	OPGLFN(glDrawElements(GL_TRIANGLES, (GLint)count, OPindexSizeToGL(OPRENDERER_ACTIVE->OPINDEXBUFFER_ACTIVE->ElementSize), (void*)(offset * sizeof(GLuint))));
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
	vertexArray->SetLayout = OPvertexArrayGLSetLayout;
	vertexArray->_Draw = OPvertexArrayGLDraw;
	vertexArray->_DrawIndexed = OPvertexArrayGLDrawIndexed;
	vertexArray->Unbind = OPvertexArrayGLUnbind;
	vertexArray->Destroy = OPvertexArrayGLDestroy;
}