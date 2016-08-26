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

	inline void Init(OPtextureDesc textureDesc) {
		OPRENDERER_ACTIVE->FrameBuffer.Init(this, textureDesc);
	}

	inline void Init(OPtextureDesc* textureDesc, ui32 count) {
		OPRENDERER_ACTIVE->FrameBuffer.Init(this, textureDesc, count);
	}

	inline void Bind() {
		OPRENDERER_ACTIVE->FrameBuffer.Bind(this);
	}

	inline void Unbind() {
		OPRENDERER_ACTIVE->FrameBuffer.Unbind(this);
	}

	inline void Destroy() {
		OPRENDERER_ACTIVE->FrameBuffer.Destroy(this);
	}
};