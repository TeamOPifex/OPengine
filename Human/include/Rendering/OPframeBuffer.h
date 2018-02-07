#pragma once

struct OPframeBuffer;
typedef struct OPframeBuffer OPframeBuffer;

#include "./Human/include/Rendering/OPtexture.h"
#include "OPrender.h"
#include "./Core/include/OPtypes.h"
#include "./Human/include/Rendering/OPrenderBuffer.h"

extern OPframeBuffer* OPRENDER_CURR_FRAMEBUFFER;



struct OPframeBuffer {
	void* internalPtr;

	OPrenderBuffer* renderBuffer = NULL;

	OPtexture* texture;
	ui32 count;
	OPtexture depthTexture;
	OPtextureDesc desc;

	inline void Init(OPtextureDesc textureDesc, bool depth = true) {
		OPRENDERER_ACTIVE->FrameBuffer.Init(this, textureDesc, depth);
	}

	inline void Init(OPtextureDesc textureDesc, OPtextureDesc depthDesc) {
		OPRENDERER_ACTIVE->FrameBuffer.Init(this, textureDesc, depthDesc);
	}

	inline void Init(OPtextureDesc* textureDesc, ui32 count) {
		OPRENDERER_ACTIVE->FrameBuffer.Init(this, textureDesc, count);
	}

	inline void Init(OPtextureDesc textureDesc, OPtexture* depthTexture) {
		OPRENDERER_ACTIVE->FrameBuffer.Init(this, &textureDesc, 1, depthTexture);
	}

	inline void Init(OPtextureDesc* textureDesc, ui32 count, OPtexture* depthTexture) {
		OPRENDERER_ACTIVE->FrameBuffer.Init(this, textureDesc, count, depthTexture);
	}

	inline void SetAttachment(ui32 ind, OPtexture* texture) {
		OPRENDERER_ACTIVE->FrameBuffer.SetAttachment(this, ind, texture);
	}

	inline void Bind(ui32 mips = 0) {
		OPRENDERER_ACTIVE->FrameBuffer.Bind(this, mips);
	}

	inline void BindReadOnly(ui32 mips = 0) {
		OPRENDERER_ACTIVE->FrameBuffer.BindReadOnly(this, mips);
	}

	inline void BindWriteOnly(ui32 mips = 0) {
		OPRENDERER_ACTIVE->FrameBuffer.BindWriteOnly(this, mips);
	}

	inline void Destroy() {
		OPRENDERER_ACTIVE->FrameBuffer.Destroy(this);
	}

	static inline void Unbind() {
		OPRENDERER_ACTIVE->FrameBuffer.Unbind();
	}

	static inline void UnbindReadOnly() {
		OPRENDERER_ACTIVE->FrameBuffer.UnbindReadOnly();
	}

	static inline void UnbindWriteOnly() {
		OPRENDERER_ACTIVE->FrameBuffer.UnbindWriteOnly();
	}
};