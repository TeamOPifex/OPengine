
#include "./Human/include/Platform/opengles/OPframeBufferAPIGLES.h"

#ifdef OPIFEX_OPENGL_ES_2
#include "./Human/include/Platform/opengles/OPcommonGLES.h"
#include "./Human/include/Rendering/OPframeBuffer.h"
#include "./Human/include/Platform/opengles/OPtextureAPIGLES.h"
#include "./Human/include/Rendering/OPwindow.h"


OPframeBuffer* _OPframeBufferAPIGLESInit(OPframeBuffer* framebuffer, OPtextureDesc textureDesc) {
	OPframeBufferGLES* frameBufferGL = OPNEW(OPframeBufferGLES());
	framebuffer->internalPtr = frameBufferGL;
	framebuffer->count = 1;

	OPGLFN(glGenFramebuffers(1, &frameBufferGL->Handle));
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferGL->Handle));

	framebuffer->texture = OPRENDERER_ACTIVE->Texture.Create(textureDesc);
	OPtextureGLES* textureGL = (OPtextureGLES*)framebuffer->texture->internalPtr;

	// Texture Buffer
	textureDesc.mipmap = false;


	if (!textureDesc.multisampled) {
		OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureGL->Handle, 0));
	}
	else {

		// OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureGL->Handle, 0));
	}


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		OPlogErr("Framebuffer not complete!");

	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	return framebuffer;
}

OPframeBuffer* _OPframeBufferAPIGLESInit(OPframeBuffer* framebuffer, OPtextureDesc textureDesc, OPtextureDesc depthDesc) {
	OPframeBufferGLES* frameBufferGL = OPNEW(OPframeBufferGLES());
	framebuffer->internalPtr = frameBufferGL;
	framebuffer->count = 1;

	OPGLFN(glGenFramebuffers(1, &frameBufferGL->Handle));
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferGL->Handle));

	framebuffer->texture = OPRENDERER_ACTIVE->Texture.Create(textureDesc);
	OPtextureGLES* textureGL = (OPtextureGLES*)framebuffer->texture->internalPtr;

	// Texture Buffer
	textureDesc.mipmap = false;

	OPRENDERER_ACTIVE->Texture.Init(&framebuffer->depthTexture, depthDesc);
	

	if (!textureDesc.multisampled) {
		OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureGL->Handle, 0));
		
		// Depth Buffer
		OPtextureGLES* depthTextureGL = (OPtextureGLES*)framebuffer->depthTexture.internalPtr;
		OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureGL->Handle, 0));
	}
	else {

		//OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureGL->Handle, 0));

		// Depth Buffer
		OPGLFN(glGenRenderbuffers(1, &frameBufferGL->DepthHandle));
		OPGLFN(glBindRenderbuffer(GL_RENDERBUFFER, frameBufferGL->DepthHandle));
		//OPGLFN(glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, textureDesc.width, textureDesc.height));
		OPGLFN(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, frameBufferGL->DepthHandle));
	}


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		OPlogErr("Framebuffer not complete!");

	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	return framebuffer;
}

OPframeBuffer* OPframeBufferAPIGLESInitialize(OPframeBuffer* framebuffer, OPtextureDesc textureDesc, bool depth = true) {

	if (depth) {
		OPtextureDesc depthDesc = OPtextureDesc();
		depthDesc.minfilter = OPtextureFilter::NEAREST;
		depthDesc.magfilter = OPtextureFilter::NEAREST;
		depthDesc.format = OPtextureFormat::DEPTH;
		depthDesc.internalFormat = OPtextureFormat::DEPTH32F;
		depthDesc.width = textureDesc.width;
		depthDesc.height = textureDesc.height;
		depthDesc.textureType = OPtextureType::BYTE;
		depthDesc.multisampled = textureDesc.multisampled;
		depthDesc.wrap = OPtextureWrap::CLAMP_TO_BORDER;
		depthDesc.mipmap = false;

		return _OPframeBufferAPIGLESInit(framebuffer, textureDesc, depthDesc);
	}
	else {
		return _OPframeBufferAPIGLESInit(framebuffer, textureDesc);
	}
}

OPframeBuffer* OPframeBufferAPIGLESInitializeDepth(OPframeBuffer* framebuffer, OPtextureDesc textureDesc, OPtextureDesc depthDesc) {
	return _OPframeBufferAPIGLESInit(framebuffer, textureDesc, depthDesc);
}

OPframeBuffer* OPframeBufferAPIGLESInitMultiDepth(OPframeBuffer* framebuffer, OPtextureDesc* textureDesc, ui32 count, OPtexture* depthTexture) {

	OPframeBufferGLES* frameBufferGL = OPNEW(OPframeBufferGLES());
	framebuffer->internalPtr = frameBufferGL;
	framebuffer->count = count;
	framebuffer->texture = OPALLOC(OPtexture, count);

	OPGLFN(glGenFramebuffers(1, &frameBufferGL->Handle));
	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferGL->Handle));

	for (ui32 i = 0; i < count; i++) {
		//textureDesc[i].multisampled = true;
		OPRENDERER_ACTIVE->Texture.Init(&framebuffer->texture[i], textureDesc[i]);
		OPtextureGLES* textureGL = (OPtextureGLES*)framebuffer->texture[i].internalPtr;
		OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureGL->Handle, 0));
		//OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, textureGL->Handle, 0));
	}

	GLuint attachments[10] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT0 + 1, GL_COLOR_ATTACHMENT0 + 2, GL_COLOR_ATTACHMENT0 + 3, GL_COLOR_ATTACHMENT0 + 4, GL_COLOR_ATTACHMENT0 + 5, GL_COLOR_ATTACHMENT0 + 6, GL_COLOR_ATTACHMENT0 + 7, GL_COLOR_ATTACHMENT0 + 8, GL_COLOR_ATTACHMENT0 + 9 };
	//OPGLFN(glDrawBuffers(count, attachments));


	framebuffer->depthTexture = *depthTexture;
	OPtextureGLES* depthTextureGL = (OPtextureGLES*)framebuffer->depthTexture.internalPtr;
	OPGLFN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureGL->Handle, 0));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		OPlogErr("Framebuffer not complete!");

	OPGLFN(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	return framebuffer;
}

OPframeBuffer* OPframeBufferAPIGLESInitMulti(OPframeBuffer* framebuffer, OPtextureDesc* textureDesc, ui32 count) {
	OPtextureDesc depthDesc = OPtextureDesc();
	depthDesc.minfilter = OPtextureFilter::NEAREST;
	depthDesc.magfilter = OPtextureFilter::NEAREST;
	depthDesc.format = OPtextureFormat::DEPTH;
	depthDesc.internalFormat = OPtextureFormat::DEPTH32F;
	depthDesc.width = textureDesc[0].width;
	depthDesc.height = textureDesc[0].height;
	depthDesc.textureType = OPtextureType::BYTE;
	depthDesc.multisampled = false;
    depthDesc.wrap = OPtextureWrap::CLAMP_TO_BORDER;
	depthDesc.mipmap = false;

	OPtexture depthTexture;
	OPRENDERER_ACTIVE->Texture.Init(&depthTexture, depthDesc);

	return OPframeBufferAPIGLESInitMultiDepth(framebuffer, textureDesc, count, &depthTexture);
}

OPframeBuffer* OPframeBufferAPIGLESCreate(OPtextureDesc textureDesc) {
	OPframeBuffer* frameBuffer = OPNEW(OPframeBuffer());
	return OPframeBufferAPIGLESInitialize(frameBuffer, textureDesc);
}

ui32 OPframeBufferModeToGLES(OPframeBufferMode::Enum mode) {
//	switch (mode)
//	{
//	case OPframeBufferMode::BOTH: return GL_FRAMEBUFFER;
//	case OPframeBufferMode::READ: return GL_READ_FRAMEBUFFER;
//	case OPframeBufferMode::DRAW: return GL_DRAW_FRAMEBUFFER;
//	}
	return 0;
}

void OPframeBufferAPIGLESUnbind(OPframeBufferMode::Enum mode) {
	OPGLFN(glBindFramebuffer(OPframeBufferModeToGLES(mode), 0));
	OPGLFN(glViewport(0, 0, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->WindowWidth, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->WindowHeight));
	OPRENDERER_ACTIVE->OPFRAMEBUFFER_ACTIVE = NULL;
}

void OPframeBufferAPIGLESBind(OPframeBufferMode::Enum mode, OPframeBuffer* ptr) {
	if (ptr == NULL) {
		return OPframeBufferAPIGLESUnbind(mode);
	}
	OPframeBufferGLES* frameBufferGL = (OPframeBufferGLES*)ptr->internalPtr;
	OPGLFN(glBindFramebuffer(OPframeBufferModeToGLES(mode), frameBufferGL->Handle));
	OPGLFN(glViewport(0, 0, ptr->texture[0].textureDesc.width, ptr->texture[0].textureDesc.height));
	OPRENDERER_ACTIVE->OPFRAMEBUFFER_ACTIVE = ptr;
}

void OPframeBufferAPIGLESDestroy(OPframeBuffer* ptr) {
	OPframeBufferGLES* frameBufferGL = (OPframeBufferGLES*)ptr->internalPtr;
	OPGLFN(glDeleteFramebuffers(1, &frameBufferGL->Handle));
	OPfree(frameBufferGL);
	ptr->internalPtr = NULL;
}

void OPframeBufferAPIGLESInit(OPframeBufferAPI* frameBuffer) {
	frameBuffer->_Bind = OPframeBufferAPIGLESBind;
	frameBuffer->Destroy = OPframeBufferAPIGLESDestroy;
	frameBuffer->_Unbind = OPframeBufferAPIGLESUnbind;
	frameBuffer->_Create = OPframeBufferAPIGLESCreate;
	frameBuffer->_Init = OPframeBufferAPIGLESInitialize;
	frameBuffer->_InitMulti = OPframeBufferAPIGLESInitMulti;
	frameBuffer->_InitDepth = OPframeBufferAPIGLESInitializeDepth;
	frameBuffer->_InitMultiDepth = OPframeBufferAPIGLESInitMultiDepth;
}

#endif