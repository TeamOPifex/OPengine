#pragma once

struct OPframeBuffer;
typedef struct OPframeBuffer OPframeBuffer;

#include "./Human/include/Rendering/OPtexture.h"
#include "OPrender.h"
#include "./Core/include/OPtypes.h"

extern OPframeBuffer* OPRENDER_CURR_FRAMEBUFFER;



struct OPframeBuffer {
	void* internalPtr;

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

	inline void Bind(ui32 mip = 0) {
		OPRENDERER_ACTIVE->FrameBuffer.Bind(this, mip);
	}

	inline void Bind(OPframeBufferMode::Enum mode) {
		OPRENDERER_ACTIVE->FrameBuffer.Bind(mode, this);
	}

	inline void Destroy() {
		OPRENDERER_ACTIVE->FrameBuffer.Destroy(this);
	}

	static inline void Unbind() {
		OPRENDERER_ACTIVE->FrameBuffer.Unbind(OPframeBufferMode::BOTH);
	}

	static inline void Unbind(OPframeBufferMode::Enum mode) {
		OPRENDERER_ACTIVE->FrameBuffer.Unbind(mode);
	}
};