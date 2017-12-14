
#include "./Human/include/Platform/opengles/OPvertexArrayAPIGLES.h"

#ifdef OPIFEX_OPENGL_ES_2

#include "./Human/include/Platform/opengles/OPcommonGLES.h"
#include "./Human/include/Rendering/API/OPrenderer.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/OPvertexArray.h"
#include "./Human/include/Rendering/OPvertexLayout.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Core/include/OPmemory.h"

void OPvertexArrayGLESBind(OPvertexArray* vertexArray);
void OPvertexArrayGLESUnbind(OPvertexArray* ptr);

GLint OPshaderElementTypeToGLES(OPshaderElementType shaderElementType) {
	switch (shaderElementType) {
	case OPshaderElementType::FLOAT: return GL_FLOAT;
	case OPshaderElementType::INT: return GL_INT;
	case OPshaderElementType::SHORT: return GL_SHORT;
	}
	return 0;
}

OPvertexArray* OPvertexArrayGLESInit(OPvertexArray* vertexArray, OPvertexLayout* vertexLayout) {
	OPvertexArrayGLES* vertexArrayGL = (OPvertexArrayGLES*)OPalloc(sizeof(OPvertexArrayGLES));
	vertexArray->internalPtr = vertexArrayGL;
	//vertexArray->vertexLayout = vertexLayout;
	OPGLFN(glGenVertexArrays(1, &vertexArrayGL->Handle));

	return vertexArray;
}

OPvertexArray* OPvertexArrayGLESCreate(OPvertexLayout* vertexLayout) {
	OPvertexArray* vertexBuffer = (OPvertexArray*)OPalloc(sizeof(OPvertexArray));
	return OPvertexArrayGLESInit(vertexBuffer, vertexLayout);
}

void OPvertexArrayGLESBind(OPvertexArray* vertexArray) {
	OPvertexArrayGLES* vertexArrayGL = (OPvertexArrayGLES*)vertexArray->internalPtr;
	OPGLFN(glBindVertexArray(vertexArrayGL->Handle));
	OPRENDERER_ACTIVE->OPVERTEXARRAY_ACTIVE = vertexArray;
}

#include "./Human/include/Platform/opengles/OPeffectAPIGLES.h"
void OPvertexArrayGLESSetLayout(OPvertexArray* vertexArray, OPvertexLayout* vertexLayout) {
	OPvertexArrayGLESBind(vertexArray);
	
	ui32 i = 0;
	for (; i < vertexLayout->count; i++)
	{
		OPshaderAttribute shaderAttribute = vertexLayout->attributes[i];
		if (shaderAttribute.Location < 0) {
			if (OPRENDERER_ACTIVE->OPEFFECT_ACTIVE != NULL) {
				OPeffectGLES* effectGL = (OPeffectGLES*)OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->internalPtr;
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

void OPvertexArrayGLESDraw(OPvertexArray* vertexArray, OPuint count, OPuint offset) {
	OPvertexArrayGLES* vertexArrayGL = (OPvertexArrayGLES*)vertexArray->internalPtr;
	OPGLFN(glDrawArrays(GL_TRIANGLES, (GLint)offset, (GLsizei)count));
}

ui32 OPindexSizeToGLES(OPindexSize indexSize) {
	switch (indexSize) {
		case OPindexSize::BYTE: return GL_UNSIGNED_BYTE;
		case OPindexSize::SHORT: return GL_UNSIGNED_SHORT;
		case OPindexSize::INT: return GL_UNSIGNED_INT;
	}
	return 0;
}

#include "./Human/include/Rendering/OPindexBuffer.h"
void OPvertexArrayGLDrawIndexed(OPvertexArray* vertexArray, OPuint count, OPuint offset) {
	OPvertexArrayGLES* vertexArrayGL = (OPvertexArrayGLES*)vertexArray->internalPtr;	
	OPGLFN(glDrawElements(GL_TRIANGLES, (GLint)count, OPindexSizeToGL(OPRENDERER_ACTIVE->OPINDEXBUFFER_ACTIVE->ElementSize), (void*)(offset * sizeof(GLuint))));
}

void OPvertexArrayGLESUnbind(OPvertexArray* ptr) {
	OPGLFN(glBindVertexArray(0));
	OPRENDERER_ACTIVE->OPVERTEXARRAY_ACTIVE = NULL;
}

void OPvertexArrayGLESDestroy(OPvertexArray* vertexArray) {
	OPvertexArrayGLES* vertexArrayGL = (OPvertexArrayGLES*)vertexArray->internalPtr;
	OPGLFN(glDeleteVertexArrays(1, &vertexArrayGL->Handle));
	OPfree(vertexArrayGL);
	vertexArray->internalPtr = NULL;
}

void OPvertexArrayAPIGLESInit(OPvertexArrayAPI* vertexArray) {
	vertexArray->Init = OPvertexArrayGLESInit;
	vertexArray->Create = OPvertexArrayGLESCreate;
	vertexArray->Bind = OPvertexArrayGLESBind;
	vertexArray->SetLayout = OPvertexArrayGLESSetLayout;
	vertexArray->_Draw = OPvertexArrayGLESDraw;
	vertexArray->_DrawIndexed = OPvertexArrayGLESDrawIndexed;
	vertexArray->Unbind = OPvertexArrayGLESUnbind;
	vertexArray->Destroy = OPvertexArrayGLESDestroy;
}

#endif