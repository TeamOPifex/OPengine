#pragma once

struct OPframeBufferDepthAPI;
typedef struct OPframeBufferDepthAPI OPframeBufferDepthAPI;

#include "./Core/include/OPtypes.h"
#include "./Human/include/Rendering/OPtextureDesc.h"
struct OPtexture;
struct OPframeBufferDepth;

struct OPframeBufferDepthAPI {
	OPframeBufferDepth*(*_Create)(OPtextureDesc textureDesc) = 0;
	OPframeBufferDepth*(*_Init)(OPframeBufferDepth* framebuffer, OPtextureDesc textureDesc) = 0;
	void(*Bind)(OPframeBufferDepth* ptr) = 0;
	void(*Unbind)(OPframeBufferDepth* ptr) = 0;
	void(*Destroy)(OPframeBufferDepth* ptr) = 0;

	inline OPframeBufferDepth* Init(OPframeBufferDepth* frameBuffer, OPtextureDesc textureDesc) {
		return _Init(frameBuffer, textureDesc);
	}
};