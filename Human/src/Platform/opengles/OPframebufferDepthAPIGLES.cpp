#include "./Human/include/Platform/opengles/OPframeBufferDepthAPIGLES.h"

#ifdef OPIFEX_OPENGL_ES_2

#include "./Human/include/Platform/opengles/OPcommonGLES.h"
#include "./Human/include/Rendering/OPframeBufferDepth.h"
#include "./Human/include/Platform/opengles/OPtextureAPIGLES.h"
#include "./Human/include/Rendering/OPwindow.h"

OPframeBufferDepth* OPframeBufferDepthAPIGLESInit(OPframeBufferDepth* framebuffer, OPtextureDesc textureDesc) {
	OPframeBufferDepthGLES* frameBufferGL = OPNEW(OPframeBufferDepthGLES());
	framebuffer->internalPtr = frameBufferGL;

	OPGLFN(glGenFramebuffers(1, &frameBufferGL->Handle));
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferGL->Handle));

	// Texture Buffer
	OPRENDERER_ACTIVE->Texture.Init(&framebuffer->texture, textureDesc);
	OPtextureGLES* textureGL = (OPtextureGLES*)framebuffer->texture.internalPtr;

	// TODO: (garrett) move this into the OPtextureDesc
	framebuffer->texture.Bind(0);
	//OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
	//OPGLFN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE));

	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferGL->Handle));
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat borderColor[] = { 0.0, 0.0, 0.0, 1.0 };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//OPGLFN(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureGL->Handle, 0));

	//OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureGL->Handle, 0));
	OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureGL->Handle, 0));
	//OPGLFN(glDrawBuffer(GL_NONE));
	//OPGLFN(glReadBuffer(GL_NONE));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		OPlogErr("Framebuffer not complete!");

	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	return framebuffer;
}

OPframeBufferDepth* OPframeBufferDepthAPIGLESCreate(OPtextureDesc textureDesc) {
	OPframeBufferDepth* frameBuffer = OPNEW(OPframeBufferDepth());
	return OPframeBufferDepthAPIGLESInit(frameBuffer, textureDesc);
}

void OPframeBufferDepthAPIGLESBind(OPframeBufferDepth* ptr) {
	OPframeBufferDepthGLES* frameBufferGL = (OPframeBufferDepthGLES*)ptr->internalPtr;
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferGL->Handle));
	OPGLFN(glViewport(0, 0, ptr->texture.textureDesc.width, ptr->texture.textureDesc.height));
}

void OPframeBufferDepthAPIGLESUnbind(OPframeBufferDepth* ptr) {
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	OPGLFN(glViewport(0, 0, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->WindowWidth, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->WindowHeight));
}

void OPframeBufferDepthAPIGLESDestroy(OPframeBufferDepth* ptr) {
	OPframeBufferDepthGLES* frameBufferGL = (OPframeBufferDepthGLES*)ptr->internalPtr;
	ptr->texture.Destroy();
	OPGLFN(glDeleteFramebuffers(1, &frameBufferGL->Handle));
	OPfree(frameBufferGL);
	ptr->internalPtr = NULL;
}

void OPframeBufferDepthAPIGLESInit(OPframeBufferDepthAPI* frameBuffer) {
	frameBuffer->Bind = OPframeBufferDepthAPIGLESBind;
	frameBuffer->Destroy = OPframeBufferDepthAPIGLESDestroy;
	frameBuffer->Unbind = OPframeBufferDepthAPIGLESUnbind;
	frameBuffer->_Create = OPframeBufferDepthAPIGLESCreate;
	frameBuffer->_Init = OPframeBufferDepthAPIGLESInit;
}

#endif