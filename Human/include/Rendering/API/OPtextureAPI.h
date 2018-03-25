#pragma once

struct OPtextureAPI;
typedef struct OPtextureAPI OPtextureAPI;

#include "./Human/include/Rendering/OPtextureDesc.h"
struct OPtexture;

struct OPtextureAPI {
	OPtexture*(*_Create)(OPtextureDesc textureDesc, const void* pixelData) = 0;
	OPtexture*(*_Init)(OPtexture* texture, OPtextureDesc textureDesc, const void* pixelData) = 0;
	void(*SetData)(OPtexture* ptr, const void* pixelData) = 0;
	void(*Bind)(OPtexture* ptr, ui32 slot) = 0;
	void(*Unbind)(OPtexture* ptr, ui32 slot) = 0;
	void(*Destroy)(OPtexture* ptr) = 0;
	void(*SetMinFilter)(OPtexture* ptr, OPtextureFilter::Enum filter) = 0;
	void(*SetMagFilter)(OPtexture* ptr, OPtextureFilter::Enum filter) = 0;

	inline OPtexture* Init(OPtexture* texture, OPtextureDesc textureDesc) {
		return _Init(texture, textureDesc, NULL);
	}
	inline OPtexture* Init(OPtexture* texture, OPtextureDesc textureDesc, const void* pixelData) {
		return _Init(texture, textureDesc, pixelData);
	}

	inline OPtexture* Create(OPtextureDesc textureDesc) {
		return _Create(textureDesc, NULL);
	}
	inline OPtexture* Create(OPtextureDesc textureDesc, const void* pixelData) {
		return _Create(textureDesc, pixelData);
	}
};