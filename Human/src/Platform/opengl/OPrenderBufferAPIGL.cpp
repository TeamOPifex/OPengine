#include "./Human/include/Platform/opengl/OPrenderBufferAPIGL.h"

#ifdef OPIFEX_OPENGL

#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPrenderBuffer.h"


OPrenderBuffer* OPrenderBufferAPIGLInit(OPrenderBuffer* renderBuffer) {
	OPrenderBufferGL* renderBufferGL = OPNEW(OPrenderBufferGL());
	renderBuffer->internalPtr = renderBufferGL;

	glGenRenderbuffers(1, &renderBufferGL->Handle);	

	return renderBuffer;
}

OPrenderBuffer* OPrenderBufferAPIGLCreate() {
	OPrenderBuffer* renderBuffer = OPNEW(OPrenderBuffer());
	return OPrenderBufferAPIGLInit(renderBuffer);
}

void OPrenderBufferAPIGLBind(OPrenderBuffer* ptr) {
	OPrenderBufferGL* renderBufferGL = (OPrenderBufferGL*)ptr->internalPtr;

	OPGLFN(glBindRenderbuffer(GL_RENDERBUFFER, renderBufferGL->Handle));
}

void OPrenderBufferAPIGLSetSize(OPrenderBuffer* ptr, ui32 width, ui32 height) {
	OPrenderBufferAPIGLBind(ptr);
	if (ptr->samples > 1) {
		OPGLFN(glRenderbufferStorageMultisample(GL_RENDERBUFFER, ptr->samples, GL_DEPTH_COMPONENT, width, height));
	}
	else {
		OPGLFN(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height));
	}
}

void OPrenderBufferAPIGLUnbind() {
	OPGLFN(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void OPrenderBufferAPIGLDestroy(OPrenderBuffer* ptr) {
	OPrenderBufferGL* renderBufferGL = (OPrenderBufferGL*)ptr->internalPtr;
	OPGLFN(glDeleteRenderbuffers(1, &renderBufferGL->Handle));
	OPfree(renderBufferGL);
	ptr->internalPtr = NULL;
}


void OPrenderBufferAPIGLInit(OPrenderBufferAPI* renderBuffer) {
	renderBuffer->Create = OPrenderBufferAPIGLCreate;
	renderBuffer->Init = OPrenderBufferAPIGLInit;
	renderBuffer->Bind = OPrenderBufferAPIGLBind;
	renderBuffer->SetSize = OPrenderBufferAPIGLSetSize;
	renderBuffer->Unbind = OPrenderBufferAPIGLUnbind;
	renderBuffer->Destroy = OPrenderBufferAPIGLDestroy;
}

#endif