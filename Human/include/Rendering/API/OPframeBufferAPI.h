#pragma once

struct OPframeBufferAPI;
typedef struct OPframeBufferAPI OPframeBufferAPI;

#include "./Core/include/OPtypes.h"
#include "./Human/include/Rendering/OPtextureDesc.h"
#include "./Human/include/Rendering/Enums/OPrendererType.h"
struct OPtexture;
struct OPframeBuffer;

struct OPframeBufferAPI {
	OPframeBuffer*(*_Create)(OPtextureDesc textureDesc) = 0;
	OPframeBuffer*(*_Init)(OPframeBuffer* framebuffer, OPtextureDesc textureDesc, bool depth) = 0;
	OPframeBuffer*(*_InitMulti)(OPframeBuffer* framebuffer, OPtextureDesc* textureDesc, ui32 count) = 0;
	OPframeBuffer*(*_InitDepth)(OPframeBuffer* framebuffer, OPtextureDesc textureDesc, OPtextureDesc depthDesc) = 0;
	OPframeBuffer*(*_InitMultiDepth)(OPframeBuffer* framebuffer, OPtextureDesc* textureDesc, ui32 count, OPtexture* depthTexture) = 0;
	void(*SetAttachment)(OPframeBuffer* frameBuffer, ui32 ind, OPtexture* texture) = 0;
	void(*_Bind)(OPframeBufferMode::Enum mode, ui32 mip, OPframeBuffer* ptr) = 0;
	void(*_Unbind)(OPframeBufferMode::Enum mode) = 0;
	void(*Destroy)(OPframeBuffer* ptr) = 0;

	inline OPframeBuffer* Init(OPframeBuffer* frameBuffer, OPtextureDesc textureDesc, bool depth = true) {
		return _Init(frameBuffer, textureDesc, depth);
	}

	inline OPframeBuffer* Init(OPframeBuffer* frameBuffer, OPtextureDesc* textureDesc, ui32 count) {
		return _InitMulti(frameBuffer, textureDesc, count);
	}

	inline OPframeBuffer* Init(OPframeBuffer* frameBuffer, OPtextureDesc textureDesc, OPtextureDesc depthDesc) {
		return _InitDepth(frameBuffer, textureDesc, depthDesc);
	}

	inline OPframeBuffer* Init(OPframeBuffer* frameBuffer, OPtextureDesc* textureDesc, ui32 count, OPtexture* depthTexture) {
		return _InitMultiDepth(frameBuffer, textureDesc, count, depthTexture);
	}

	inline void Bind(OPframeBuffer* ptr, ui32 mip = 0) {
		_Bind(OPframeBufferMode::BOTH, mip, ptr);
	}

	inline void Bind(OPframeBufferMode::Enum mode, OPframeBuffer* ptr) {
		_Bind(mode, 0, ptr);
	}

	inline void Unbind() {
		_Unbind(OPframeBufferMode::BOTH);
	}

	inline void Unbind(OPframeBufferMode::Enum mode) {
		_Unbind(mode);
	}
};