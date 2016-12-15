#include "./Human/include/Platform/opengl/OPframeBufferAPIGL.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPframeBuffer.h"
#include "./Human/include/Platform/opengl/OPtextureAPIGL.h"
#include "./Human/include/Rendering/OPwindow.h"

OPframeBuffer* OPframeBufferAPIGLInitDepth(OPframeBuffer* framebuffer, OPtextureDesc textureDesc, OPtexture* depthTexture) {
	OPframeBufferGL* frameBufferGL = OPNEW(OPframeBufferGL());
	framebuffer->internalPtr = frameBufferGL;
	framebuffer->count = 1;

	OPGLFN(glGenFramebuffers(1, &frameBufferGL->Handle));

	// Texture Buffer
	framebuffer->texture = OPRENDERER_ACTIVE->Texture.Create(textureDesc);
	OPtextureGL* textureGL = (OPtextureGL*)framebuffer->texture->internalPtr;
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferGL->Handle));
	OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureGL->Handle, 0));

	// Depth Buffer
	framebuffer->depthTexture = *depthTexture;
	OPtextureGL* depthTextureGL = (OPtextureGL*)framebuffer->depthTexture.internalPtr;
	OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureGL->Handle, 0));


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		OPlogErr("Framebuffer not complete!");

	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	return framebuffer;
}

OPframeBuffer* OPframeBufferAPIGLInit(OPframeBuffer* framebuffer, OPtextureDesc textureDesc) {
	OPtextureDesc depthDesc;
	depthDesc.filter = OPtextureFilter::NEAREST;
	depthDesc.format = OPtextureFormat::DEPTH;
	depthDesc.internalFormat = OPtextureFormat::DEPTH32F;
	depthDesc.width = textureDesc.width;
	depthDesc.height = textureDesc.height;
	depthDesc.textureType = OPtextureType::BYTE;

	OPtexture depthTexture;
	OPRENDERER_ACTIVE->Texture.Init(&depthTexture, depthDesc);

	return OPframeBufferAPIGLInitDepth(framebuffer, textureDesc, &depthTexture);
}

OPframeBuffer* OPframeBufferAPIGLInitMultiDepth(OPframeBuffer* framebuffer, OPtextureDesc* textureDesc, ui32 count, OPtexture* depthTexture) {

	OPframeBufferGL* frameBufferGL = OPNEW(OPframeBufferGL());
	framebuffer->internalPtr = frameBufferGL;
	framebuffer->count = count;
	framebuffer->texture = OPALLOC(OPtexture, count);

	OPGLFN(glGenFramebuffers(1, &frameBufferGL->Handle));
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferGL->Handle));

	for (ui32 i = 0; i < count; i++) {
		OPRENDERER_ACTIVE->Texture.Init(&framebuffer->texture[i], textureDesc[i]);
		OPtextureGL* textureGL = (OPtextureGL*)framebuffer->texture[i].internalPtr;
		OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureGL->Handle, 0));
	}

	GLuint attachments[10] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7, GL_COLOR_ATTACHMENT8, GL_COLOR_ATTACHMENT9 };
	OPGLFN(glDrawBuffers(count, attachments));


	framebuffer->depthTexture = *depthTexture;
	OPtextureGL* depthTextureGL = (OPtextureGL*)framebuffer->depthTexture.internalPtr;
	OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureGL->Handle, 0));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		OPlogErr("Framebuffer not complete!");

	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	return framebuffer;
}

OPframeBuffer* OPframeBufferAPIGLInitMulti(OPframeBuffer* framebuffer, OPtextureDesc* textureDesc, ui32 count) {
	OPtextureDesc depthDesc;
	depthDesc.filter = OPtextureFilter::NEAREST;
	depthDesc.format = OPtextureFormat::DEPTH;
	depthDesc.internalFormat = OPtextureFormat::DEPTH32F;
	depthDesc.width = textureDesc[0].width;
	depthDesc.height = textureDesc[0].height;
	depthDesc.textureType = OPtextureType::BYTE;

	OPtexture depthTexture;
	OPRENDERER_ACTIVE->Texture.Init(&depthTexture, depthDesc);

	return OPframeBufferAPIGLInitMultiDepth(framebuffer, textureDesc, count, &depthTexture);
}

OPframeBuffer* OPframeBufferAPIGLCreate(OPtextureDesc textureDesc) {
	OPframeBuffer* frameBuffer = OPNEW(OPframeBuffer());
	return OPframeBufferAPIGLInit(frameBuffer, textureDesc);
}

void OPframeBufferAPIGLBind(OPframeBuffer* ptr) {
	OPframeBufferGL* frameBufferGL = (OPframeBufferGL*)ptr->internalPtr;
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferGL->Handle));
	OPGLFN(glViewport(0, 0, ptr->texture[0].textureDesc.width, ptr->texture[0].textureDesc.height));
}

void OPframeBufferAPIGLUnbind(OPframeBuffer* ptr) {
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	OPGLFN(glViewport(0, 0, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->WindowHeight, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->WindowHeight));
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
	frameBuffer->_InitMulti = OPframeBufferAPIGLInitMulti;
	frameBuffer->_InitDepth = OPframeBufferAPIGLInitDepth;
	frameBuffer->_InitMultiDepth = OPframeBufferAPIGLInitMultiDepth;
}
