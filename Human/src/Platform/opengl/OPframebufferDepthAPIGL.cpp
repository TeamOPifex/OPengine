#include "./Human/include/Platform/opengl/OPframeBufferDepthAPIGL.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPframeBufferDepth.h"
#include "./Human/include/Platform/opengl/OPtextureAPIGL.h"
#include "./Human/include/Rendering/OPwindow.h"

OPframeBufferDepth* OPframeBufferDepthAPIGLInit(OPframeBufferDepth* framebuffer, OPtextureDesc textureDesc) {
	OPframeBufferDepthGL* frameBufferGL = OPNEW(OPframeBufferDepthGL());
	framebuffer->internalPtr = frameBufferGL;

	OPGLFN(glGenFramebuffers(1, &frameBufferGL->Handle));
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferGL->Handle));

	// Texture Buffer
	OPRENDERER_ACTIVE->Texture.Init(&framebuffer->texture, textureDesc);
	OPtextureGL* textureGL = (OPtextureGL*)framebuffer->texture.internalPtr;

	// TODO: (garrett) move this into the OPtextureDesc
	framebuffer->texture.Bind(0);
	//OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
	//OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE));

	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferGL->Handle));
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat borderColor[] = { 0.0, 0.0, 0.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//OPGLFN(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureGL->Handle, 0));

	//OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureGL->Handle, 0));
	OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureGL->Handle, 0));
	OPGLFN(glDrawBuffer(GL_NONE));
	OPGLFN(glReadBuffer(GL_NONE));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		OPlogErr("Framebuffer not complete!");

	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	return framebuffer;
}

OPframeBufferDepth* OPframeBufferDepthAPIGLCreate(OPtextureDesc textureDesc) {
	OPframeBufferDepth* frameBuffer = OPNEW(OPframeBufferDepth());
	return OPframeBufferDepthAPIGLInit(frameBuffer, textureDesc);
}

void OPframeBufferDepthAPIGLBind(OPframeBufferDepth* ptr) {
	OPframeBufferDepthGL* frameBufferGL = (OPframeBufferDepthGL*)ptr->internalPtr;
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferGL->Handle));
	OPGLFN(glViewport(0, 0, ptr->texture.textureDesc.width, ptr->texture.textureDesc.height));
}

void OPframeBufferDepthAPIGLUnbind(OPframeBufferDepth* ptr) {
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	OPGLFN(glViewport(0, 0, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->WindowWidth, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->WindowHeight));
}

void OPframeBufferDepthAPIGLDestroy(OPframeBufferDepth* ptr) {
	OPframeBufferDepthGL* frameBufferGL = (OPframeBufferDepthGL*)ptr->internalPtr;
	OPGLFN(glDeleteFramebuffers(1, &frameBufferGL->Handle));
	OPfree(frameBufferGL);
	ptr->internalPtr = NULL;
}

void OPframeBufferDepthAPIGLInit(OPframeBufferDepthAPI* frameBuffer) {
	frameBuffer->Bind = OPframeBufferDepthAPIGLBind;
	frameBuffer->Destroy = OPframeBufferDepthAPIGLDestroy;
	frameBuffer->Unbind = OPframeBufferDepthAPIGLUnbind;
	frameBuffer->_Create = OPframeBufferDepthAPIGLCreate;
	frameBuffer->_Init = OPframeBufferDepthAPIGLInit;
}
