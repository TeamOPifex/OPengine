#pragma once

struct OPframeBufferDepth;
typedef struct OPframeBufferDepth OPframeBufferDepth;

#include "./Human/include/Rendering/OPtexture.h"
#include "OPrender.h"
#include "./Core/include/OPtypes.h"

struct OPframeBufferDepth {
	void* internalPtr;
	OPtexture texture;

	inline void Init(OPtextureDesc textureDesc) {
		OPRENDERER_ACTIVE->FrameBufferDepth.Init(this, textureDesc);
	}

	inline void Bind() {
		OPRENDERER_ACTIVE->FrameBufferDepth.Bind(this);
	}

	inline void Unbind() {
		OPRENDERER_ACTIVE->FrameBufferDepth.Unbind(this);
	}

	inline void Destroy() {
		OPRENDERER_ACTIVE->FrameBufferDepth.Destroy(this);
	}
};