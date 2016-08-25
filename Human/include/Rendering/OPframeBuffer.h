#pragma once

struct OPframeBuffer;
typedef struct OPframeBuffer OPframeBuffer;

#include "./Human/include/Rendering/OPtexture.h"
#include "OPrender.h"
#include "./Core/include/OPtypes.h"

extern OPframeBuffer* OPRENDER_CURR_FRAMEBUFFER;

struct OPframeBuffer {
	void* internalPtr;

	OPtextureDesc textureDesc;
	OPtexture texture;

	inline void Init(OPtextureDesc textureDesc) {
		OPRENDERER_ACTIVE->FrameBuffer._Init(this, textureDesc);
	}

	inline void Bind() {
		OPRENDERER_ACTIVE->FrameBuffer.Bind(this);
	}

	inline void Unbind() {
		OPRENDERER_ACTIVE->FrameBuffer.Unbind(this);
	}
};