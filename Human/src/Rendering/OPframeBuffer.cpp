#include <include/Rendering/OPtexture.h>
#include <include/Rendering/OPframeBuffer.h>
#include "./Human/include/Rendering/OPframeBuffer.h"
#include "./Core/include/OPlog.h"
#include "./Human/include/Utilities/Errors.h"

//-----------------------------------------------------------------------------
//   _____ _       _           _
//  / ____| |     | |         | |
// | |  __| | ___ | |__   __ _| |___
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
OPframeBuffer* OPRENDER_CURR_FRAMEBUFFER;

//-----------------------------------------------------------------------------
// ______                _   _
//|  ____|              | | (_)
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

GLuint createDepthTexture(int w, int h) {

	GLuint tex;
#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS)
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
						w,
						h,
						0, GL_DEPTH_COMPONENT, GL_FLOAT,
						NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
#endif
	return tex;
}

OPframeBuffer OPframeBufferCreateShadow(ui32 width, ui32 height) {
	OPframeBuffer fb;
#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS)

	// Setup the Texture Descriptor
	//fb.Description.Width = width;
	//fb.Description.Height = height;
	//fb.Description.DataType = GL_FLOAT;
	//fb.Description.Format = GL_DEPTH_COMPONENT;
	//fb.Description.InternalFormat = GL_DEPTH_COMPONENT;
	//fb.Description.MagFilter = GL_NEAREST;
	//fb.Description.MinFilter = GL_NEAREST;
	//fb.Description.WrapX = GL_CLAMP_TO_BORDER;
	//fb.Description.WrapY = GL_CLAMP_TO_BORDER;
	//fb.Description.CompareFunc = GL_LEQUAL;
	//fb.Description.CompareMode = GL_COMPARE_R_TO_TEXTURE;

	// Create the Framebuffer
	glGenFramebuffers(1, &fb.Handle);

	// Create a nex Depth Texture
	//fb.Texture =  OPtextureCreate(fb.Description);
	OPRENDERER_ACTIVE->Texture.Init(&fb.Texture, fb.Description);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Bind this framebuffer as the current one
	glBindFramebuffer(GL_FRAMEBUFFER, fb.Handle);

	// Attach the created depth texture as the depth buffer for this Framebuffer
	// TODO: (garrett) implement with OPframeBufferAPI
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fb.Texture.Handle, 0);

	// Turn off the color buffer
	// (For shadows we only need depth information)
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		OPlog("Frame buffer NOT OK");
	}

	// Set the current framebuffer back to the display
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

	return fb;
}

OPframeBuffer OPframeBufferCreateDepth(OPtextureDesc desc) {
	OPframeBuffer fb = {
		desc
	};

#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS)
	OPlog("OPframeBufferCreateDepth 1");
	OPglError("OPframeBufferCreateDepth:Error 1:%d");

	glGenFramebuffers(1, &fb.Handle);
	OPlog("OPframeBufferCreateDepth 2");
	OPglError("OPframeBufferCreateDepth:Error 2:%d");
	glBindFramebuffer(GL_FRAMEBUFFER, fb.Handle);
	OPglError("OPframeBufferCreateDepth:Error 3:%d");
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.Handle);
	OPglError("OPframeBufferCreateDepth:Error 4:%d");
	//OPframeBufferBind(&fb);
	OPlog("OPframeBufferCreateDepth 3");

	//fb.Texture = OPtextureCreate(desc);
	OPRENDERER_ACTIVE->Texture.Init(&fb.Texture, desc);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	OPglError("OPframeBufferCreateDepth:Error 1:%d");
	OPlog("OPframeBufferCreateDepth 7");

	//OPlog("OPframeBufferCreateDepth 4: %d", fb.Texture.Handle);
	OPglError("OPframeBufferCreateDepth:Error 5:%d");
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fb.Texture.Handle, 0);
	GLenum frameBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	OPglError("OPframeBufferCreateDepth:Error 9:%d");

	// Always check that our framebuffer is ok
	if(frameBufferStatus != GL_FRAMEBUFFER_COMPLETE) {
		OPlog("!!! FrameBuffer is not ok");
		switch(frameBufferStatus) {
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				OPlog("FrameBuffer has an incomplete attachment");
				break;
			// case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
			// 	OPlog("FrameBuffer has incomplete dimensions");
			// 	break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				OPlog("FrameBuffer has an incomplete read buffer");
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				OPlog("FrameBuffer has missing attachment");
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				OPlog("FrameBuffer internal formats are unsupported");
				break;
			default:
				OPlog("FrameBuffer unknown: %d", frameBufferStatus);
				break;
		}
	}
	OPglError("OPframeBufferCreateDepth:Error 10:%d");
#else
	OPlog("TODO: Frame Buffer not completed for Android.");
#endif

	return fb;
}

// TODO: (garrett)This is sloppy and will be taken care of when I refactor OPframeBuffer
OPframeBuffer OPframeBufferCreateVR(ui32 width, ui32 height) {

	OPframeBuffer framebufferDesc;

	glGenFramebuffers(1, &framebufferDesc.RenderFramebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.RenderFramebufferId);

	glGenRenderbuffers(1, &framebufferDesc.DepthBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, framebufferDesc.DepthBufferId);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebufferDesc.DepthBufferId);

	glGenTextures(1, &framebufferDesc.RenderTextureId);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.RenderTextureId);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, width, height, true);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.RenderTextureId, 0);

	glGenFramebuffers(1, &framebufferDesc.ResolveFramebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.ResolveFramebufferId);

	glGenTextures(1, &framebufferDesc.ResolveTextureId);
	glBindTexture(GL_TEXTURE_2D, framebufferDesc.ResolveTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferDesc.ResolveTextureId, 0);

	// check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		OPlogErr("Failed to create frame buffer");
		return framebufferDesc;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return framebufferDesc;
}

OPframeBuffer OPframeBufferCreate(OPtextureDesc desc){
	OPlog("OPEngine Frame Buffer 1");
	OPframeBuffer fb = {
		desc
	};
	fb.Handle = 0;
	OPRENDERER_ACTIVE->Texture.Init(&fb.Texture, desc);

	OPlog("OPEngine Frame Buffer 2");

	// generate and bind the fbo
	glGenFramebuffers(1, &fb.Handle);
	OPlog("OPEngine Frame Buffer 3");
	// setup color texture
	//OPtextureClearActive();

	//ui32 handle = OPtextureBind(&fb.Texture);
	//OPtextureSetData(NULL);
	//glBindTexture(GL_TEXTURE_2D, handle);

	OPlog("OPEngine Frame Buffer 4");
	// attach the depth texture
	ui32 dt = createDepthTexture(desc.width, desc.height);
	OPlog("OPEngine Frame Buffer 5");

#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS)
	OPglError("OPframeBufferCreate:Error 1:%d");
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.Handle);
	OPlog("OPEngine Frame Buffer 6");
	OPglError("OPframeBufferCreate:Error 2:%d");
	//glBindRenderbuffer(GL_RENDERBUFFER, dt);
	OPglError("OPframeBufferCreate:Error 3:%d");
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, desc.Width, desc.Height);
	OPglError("OPframeBufferCreate:Error 4:%d");

	OPlog("OPEngine Frame Buffer 6.5");
	// attach the color texture

	//OPlog("OPframeBufferCreate glFramebufferTexture2D : FB Handle :%d", fb.Texture.Handle);
	//glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb.Texture.Handle, 0);
	OPlog("OPframeBufferCreate glFramebufferRenderbuffer");
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dt, 0);

	//#if defined(OPIFEX_OSX32) || defined(OPIFEX_OSX64)
	//	OPlog("OPframeBufferCreate glFramebufferTexture2D : FB Handle :%d", fb.Texture.Handle);
	//	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb.Texture.Handle, 0);
	//	OPlog("OPframeBufferCreate glFramebufferRenderbuffer");
	//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dt, 0);
	//#else
	//glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fb.Texture.Handle, 0);
	//OPlog("OPEngine Frame Buffer 7");
	//OPglError("OPframeBufferCreate:Error 5:%d");
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, dt, 0);
	//OPlog("OPEngine Frame Buffer 8");
	//OPglError("OPframeBufferCreate:Error 6:%d");
	//#endif
#else
	OPglError("OPframeBufferCreate:Error 1:%d");
	glBindFramebuffer(GL_FRAMEBUFFER, fb.Handle);
	OPglError("OPframeBufferCreate:Error 2:%d");
	//glBindRenderbuffer(GL_RENDERBUFFER, dt);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, desc.Width, desc.Height);
	OPglError("OPframeBufferCreate:Error 3");
	// attach the color texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb.Texture.Handle, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, dt);
#endif

	// check fbo creation status
#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS)
	GLenum e = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	OPlog("OPEngine Frame Buffer 9");
#else
	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
#endif
	switch (e) {
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT :
			OPlog("FBO Incomplete Attachment");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT :
			OPlog("FBO Missing Attachment");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED :
			OPlog("FBO Unsupported");
			break;
		case GL_FRAMEBUFFER_COMPLETE:
			OPlog("FBO OK");
			break;
#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS)
		case GL_FRAMEBUFFER_UNDEFINED:
			OPlog("FBO Undefined");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER :
			OPlog("FBO Incomplete Draw Buffer");
			break;
#else
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
			OPlog("FBO Incomplete Dimensions OK");
			break;
#endif
		default:
			OPlog("FBO Problem?");
	}

	OPlog("OPEngine Frame Buffer 10");
	glBindTexture(GL_TEXTURE_2D, 0);
	OPlog("OPEngine Frame Buffer 11");
	OPglError("OPframeBufferCreate:Error 7:%d");
#if !defined(OPIFEX_ANDROID) || !defined(OPIFEX_IOS)
	OPlog("OPEngine Frame Buffer 12");
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	OPlog("OPEngine Frame Buffer 13");
	OPglError("OPframeBufferCreate:Error 8:%d");
#else
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

	OPlog("OPEngine Frame Buffer 14");
	return fb;
}
//-----------------------------------------------------------------------------
void OPframeBufferDestroy(OPframeBuffer* fb){
	fb->Texture.Destroy();
	glDeleteFramebuffers(1, &fb->Handle);
}
//-----------------------------------------------------------------------------
void OPframeBufferBind(OPframeBuffer* fb){

	OPglError("OPframeBufferBind:Error 0:%d");

	//GLuint attachments[1] = {GL_COLOR_ATTACHMENT0};
	OPRENDER_CURR_FRAMEBUFFER = fb;
	glBindFramebuffer(GL_FRAMEBUFFER, fb->Handle);

	OPglError("OPframeBufferBind:Error 1:%d");
	//glDrawBuffers(1,  attachments);
	OPrenderSetViewport(0, 0, fb->Description.width, fb->Description.height);
}
//-----------------------------------------------------------------------------
void OPframeBufferBindRead(OPframeBuffer* fb){
	//GLuint attachments[1] = {GL_COLOR_ATTACHMENT0};
	OPRENDER_CURR_FRAMEBUFFER = fb;
#if !defined(OPIFEX_ANDROID) || !defined(OPIFEX_IOS)
	glBindFramebuffer(GL_FRAMEBUFFER, fb->Handle);
#else
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fb->Handle);
#endif
	//glDrawBuffers(1,  attachments);
	OPrenderSetViewport(0, 0, fb->Description.width, fb->Description.height);
}
//-----------------------------------------------------------------------------
void OPframeBufferBindTex(OPframeBuffer* fb){
	//OPtextureBind(&fb->Texture);
	OPRENDERER_ACTIVE->Texture.Bind(&fb->Texture, 0);
}
//-----------------------------------------------------------------------------
void OPframeBufferUnbind(){
	OPframeBuffer* fb = OPRENDER_CURR_FRAMEBUFFER;

	//glBindTexture(GL_TEXTURE_2D, fb->Texture.Handle);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	OPrenderResetViewport();
/*	OPlog("FBO W: %d, H: %d", fb->Description.Width, fb->Description.Height);

	glBindTexture(GL_TEXTURE_2D, fb->Texture.Handle);
	glCopyTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		0,
		0,
		fb->Description.Width,
		fb->Description.Height,
		0
	);
	glGenerateMipmap(fb->Texture.Handle);*/
}

void OPframeBufferSetReadBuffer(ui16 pos) {
#if !defined(OPIFEX_ANDROID) || !defined(OPIFEX_IOS)
	OPlog("OPframeBufferSetReadBuffer NOT SUPPORTED on Android");
#else
	glReadBuffer(GL_COLOR_ATTACHMENT0 + pos);
#endif
}

void OPframeBufferSetReadBufferDepth() {
#if !defined(OPIFEX_ANDROID) || !defined(OPIFEX_IOS)
	OPlog("OPframeBufferSetReadBufferDepth NOT SUPPORTED on Android");
#else
	glReadBuffer(GL_DEPTH_ATTACHMENT);
#endif
}

void OPframeBufferAttach(OPtexture* texture, ui16 pos) {
#if !defined(OPIFEX_ANDROID) || !defined(OPIFEX_IOS)
	OPlog("OPframeBufferAttach NOT SUPPORTED on Android");
#else
	OPtextureBind(texture);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + pos, GL_TEXTURE_2D, texture->Handle, 0);
#endif
}

void OPframeBufferAttachDepth(OPtexture* texture) {
#if !defined(OPIFEX_ANDROID) || !defined(OPIFEX_IOS)
	OPlog("OPframeBufferAttachDepth NOT SUPPORTED on Android");
#else
	OPtextureBind(texture);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->Handle, 0);
#endif
}


//void OPframeBufferBindTexture(OPtexture* texture, ui32 pos) {
//	glBindRenderbuffer(GL_RENDERBUFFER, texture->Handle);
//	glRenderbufferStorage(GL_RENDERBUFFER, texture->Description.Format, texture->Description.Width, texture->Description.Height);
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + pos, GL_RENDERBUFFER, texture->Handle);
//}
