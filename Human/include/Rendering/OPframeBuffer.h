#pragma once

struct OPframeBuffer;
typedef struct OPframeBuffer OPframeBuffer;

#include "./Human/include/Rendering/OPtexture.h"
#include "OPrender.h"
#include "./Core/include/OPtypes.h"

extern OPframeBuffer* OPRENDER_CURR_FRAMEBUFFER;

OPframeBuffer OPframeBufferCreateShadow(ui32 width, ui32 height);
OPframeBuffer OPframeBufferCreateDepth(OPtextureDesc desc);
OPframeBuffer OPframeBufferCreate(OPtextureDesc desc);
OPframeBuffer OPframeBufferCreateVR(ui32 width, ui32 height);
void OPframeBufferDestroy(OPframeBuffer* fb);
void OPframeBufferAttach(OPtexture* texture, ui16 pos);
void OPframeBufferBind(OPframeBuffer* fb);
void OPframeBufferBindRead(OPframeBuffer* fb);
void OPframeBufferSetReadBuffer(ui16 pos);
void OPframeBufferSetReadBufferDepth();
void OPframeBufferBindTex(OPframeBuffer* fb);
void OPframeBufferUnbind();
void OPframeBufferAttachDepth(OPtexture* texture);

struct OPframeBuffer {
	OPtextureDesc Description;
	OPtexture Texture;
	ui32 Handle;

	ui32 DepthBufferId;
	ui32 RenderTextureId;
	ui32 RenderFramebufferId;
	ui32 ResolveTextureId;
	ui32 ResolveFramebufferId;

	void Bind() {
		OPframeBufferBind(this);
	}

	void Unbind() {
		OPframeBufferUnbind();
	}
};