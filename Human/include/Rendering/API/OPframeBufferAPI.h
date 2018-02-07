#pragma once

struct OPframeBufferAPI;
typedef struct OPframeBufferAPI OPframeBufferAPI;

#include "./Core/include/OPtypes.h"
#include "./Human/include/Rendering/OPtextureDesc.h"
#include "./Human/include/Rendering/Enums/OPrendererType.h"
struct OPframeBuffer;
struct OPtexture;
struct OPrenderBuffer;

struct OPframeBufferAPI {
	OPframeBuffer*(*_Create)(OPtextureDesc textureDesc) = 0;
	OPframeBuffer*(*_Init)(OPframeBuffer* framebuffer, OPtextureDesc textureDesc, bool depth) = 0;
	OPframeBuffer*(*_InitMulti)(OPframeBuffer* framebuffer, OPtextureDesc* textureDesc, ui32 count) = 0;
	OPframeBuffer*(*_InitDepth)(OPframeBuffer* framebuffer, OPtextureDesc textureDesc, OPtextureDesc depthDesc) = 0;
	OPframeBuffer*(*_InitMultiDepth)(OPframeBuffer* framebuffer, OPtextureDesc* textureDesc, ui32 count, OPtexture* depthTexture) = 0;
	void(*SetAttachmentDepthTexture)(OPframeBuffer* frameBuffer, OPtexture* texture) = 0;
	void(*SetAttachmentTexture)(OPframeBuffer* frameBuffer, ui32 ind, OPtexture* texture) = 0;
	void(*SetAttachmentRenderBuffer)(OPframeBuffer* frameBuffer, OPrenderBuffer* renderBuffer) = 0;
	void(*_Bind)(OPframeBuffer* ptr, ui32 mip) = 0;
	void(*BindReadOnly)(OPframeBuffer* ptr, ui32 mip) = 0;
	void(*BindWriteOnly)(OPframeBuffer* ptr, ui32 mip) = 0;
	void(*Unbind)() = 0;
	void(*UnbindReadOnly)() = 0;
	void(*UnbindWriteOnly)() = 0;
	void(*Destroy)(OPframeBuffer* ptr) = 0;

	inline void SetAttachment(OPframeBuffer* frameBuffer, ui32 ind, OPtexture* texture) {
		SetAttachmentTexture(frameBuffer, ind, texture);
	}

	inline void SetAttachment(OPframeBuffer* frameBuffer, OPrenderBuffer* renderBuffer) {
		SetAttachmentRenderBuffer(frameBuffer, renderBuffer);
	}

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
		_Bind(ptr, mip);
	}
};