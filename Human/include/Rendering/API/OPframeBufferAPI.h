#pragma once

struct OPframeBufferAPI;
typedef struct OPframeBufferAPI OPframeBufferAPI;

#include "./Core/include/OPtypes.h"
#include "./Human/include/Rendering/OPtextureDesc.h"
struct OPtexture;
struct OPframeBuffer;

struct OPframeBufferAPI {
	OPframeBuffer*(*_Create)(OPtextureDesc textureDesc) = 0;
	OPframeBuffer*(*_Init)(OPframeBuffer* framebuffer, OPtextureDesc textureDesc) = 0;
	void(*Bind)(OPframeBuffer* ptr) = 0;
	void(*Unbind)(OPframeBuffer* ptr) = 0;
	void(*Destroy)(OPframeBuffer* ptr) = 0;
};