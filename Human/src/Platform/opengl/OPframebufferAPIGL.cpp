#include "./Human/include/Platform/opengl/OPframeBufferAPIGL.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPframeBuffer.h"
#include "./Human/include/Platform/opengl/OPtextureAPIGL.h"
#include "./Human/include/Rendering/OPwindow.h"

OPframeBuffer* OPframeBufferAPIGLInit(OPframeBuffer* framebuffer, OPtextureDesc textureDesc) {
	OPframeBufferGL* frameBufferGL = OPNEW(OPframeBufferGL());
	framebuffer->internalPtr = frameBufferGL;
	framebuffer->textureDesc = textureDesc;

	OPGLFN(glGenFramebuffers(1, &frameBufferGL->Handle));
	OPGLFN(glGenRenderbuffers(1, &frameBufferGL->DepthHandle));

	OPRENDERER_ACTIVE->Texture.Init(&framebuffer->texture, textureDesc);

	OPGLFN(glBindRenderbuffer(GL_RENDERBUFFER, frameBufferGL->DepthHandle));
	OPGLFN(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, textureDesc.width, textureDesc.height));

	OPtextureGL* textureGL = (OPtextureGL*)framebuffer->texture.internalPtr;
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferGL->Handle));
	OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureGL->Handle, 0));
	OPGLFN(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, frameBufferGL->DepthHandle));
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	return framebuffer;
}

OPframeBuffer* OPframeBufferAPIGLCreate(OPtextureDesc textureDesc) {
	OPframeBuffer* frameBuffer = OPNEW(OPframeBuffer());
	return OPframeBufferAPIGLInit(frameBuffer, textureDesc);
}

void OPframeBufferAPIGLBind(OPframeBuffer* ptr) {
	OPframeBufferGL* frameBufferGL = (OPframeBufferGL*)ptr->internalPtr;
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferGL->Handle));
	OPGLFN(glViewport(0, 0, ptr->textureDesc.width, ptr->textureDesc.height));
}

void OPframeBufferAPIGLUnbind(OPframeBuffer* ptr) {
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	OPGLFN(glViewport(0, 0, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height));
}

void OPframeBufferAPIGLDestroy(OPframeBuffer* ptr) {
	OPframeBufferGL* frameBufferGL = (OPframeBufferGL*)ptr->internalPtr;
	OPGLFN(glDeleteFramebuffers(1, &frameBufferGL->Handle));
	OPfree(frameBufferGL);
	ptr->internalPtr = NULL;
}

void OPframeBufferAPIGLInit(OPframeBufferAPI* frameBuffer) {
	frameBuffer->Bind = OPframeBufferAPIGLBind;
	frameBuffer->Destroy = OPframeBufferAPIGLDestroy;
	frameBuffer->Unbind = OPframeBufferAPIGLUnbind;
	frameBuffer->_Create = OPframeBufferAPIGLCreate;
	frameBuffer->_Init = OPframeBufferAPIGLInit;
}