#include "./Human/include/Rendering/FrameBuffer.h"
#include "./Core/include/Log.h"
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
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
						w,
						h, 
						0, GL_DEPTH_COMPONENT, GL_FLOAT,
						NULL); 
	return tex;
}

OPframeBuffer OPframeBufferCreate(OPtextureDescription desc){
	OPlog("OPEngine Frame Buffer 1");
	OPframeBuffer fb = {
		desc,
		OPtextureCreate(desc),
		0
	};
	OPlog("OPEngine Frame Buffer 2");

	// generate and bind the fbo
	glGenFramebuffers(1, &fb.Handle);
	OPlog("OPEngine Frame Buffer 3");
	// setup color texture
	OPtextureClearActive();
	ui32 handle = OPtextureBind(&fb.Texture);	
	OPtextureSetData(NULL);
	glBindTexture(GL_TEXTURE_2D, handle);
	OPlog("OPEngine Frame Buffer 4");
	// attach the depth texture
	ui32 dt = createDepthTexture(desc.Width, desc.Height);
	OPlog("OPEngine Frame Buffer 5");

#ifndef OPIFEX_ANDROID
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

	OPlog("OPframeBufferCreate glFramebufferTexture2D : FB Handle :%d", fb.Texture.Handle);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb.Texture.Handle, 0);
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
#ifndef OPIFEX_ANDROID
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
#ifndef OPIFEX_ANDROID
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
#ifndef OPIFEX_ANDROID
	OPlog("OPEngine Frame Buffer 12");
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
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
	OPtextureDestroy(&fb->Texture);
	glDeleteFramebuffers(1, &fb->Handle);
}
//-----------------------------------------------------------------------------
void OPframeBufferBind(OPframeBuffer* fb){
	//GLuint attachments[1] = {GL_COLOR_ATTACHMENT0};
	OPRENDER_CURR_FRAMEBUFFER = fb;
	glBindFramebuffer(GL_FRAMEBUFFER, fb->Handle);
	//glDrawBuffers(1,  attachments);
	OPrenderSetViewport(0, 0, fb->Description.Width, fb->Description.Height);
}
//-----------------------------------------------------------------------------
void OPframeBufferBindRead(OPframeBuffer* fb){
	//GLuint attachments[1] = {GL_COLOR_ATTACHMENT0};
	OPRENDER_CURR_FRAMEBUFFER = fb;
#ifdef OPIFEX_ANDROID
	glBindFramebuffer(GL_FRAMEBUFFER, fb->Handle);
#else
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fb->Handle);
#endif
	//glDrawBuffers(1,  attachments);
	OPrenderSetViewport(0, 0, fb->Description.Width, fb->Description.Height);
}
//-----------------------------------------------------------------------------
void OPframeBufferBindTex(OPframeBuffer* fb){
	OPtextureBind(&fb->Texture);
}
//-----------------------------------------------------------------------------                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
void OPframeBufferUnbind(){
	OPframeBuffer* fb = OPRENDER_CURR_FRAMEBUFFER;

	glBindTexture(GL_TEXTURE_2D, fb->Texture.Handle);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

