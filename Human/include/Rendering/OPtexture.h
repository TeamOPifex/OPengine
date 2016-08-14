#pragma once

struct OPtexture;
typedef struct OPtexture OPtexture;

#include "./Human/include/Rendering/OPtextureDesc.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Math/include/OPvec4.h"

struct OPtexture {
	void* internalPtr;
	OPuint guid;
	OPtextureDesc textureDesc;

	inline void SetData(const void* pixelData) {
		OPRENDERER_ACTIVE->Texture.SetData(this, pixelData);
	}

	inline void Bind(ui32 slot) {
		OPRENDERER_ACTIVE->Texture.Bind(this, slot);
	}

	inline void Unbind(ui32 slot) {
		OPRENDERER_ACTIVE->Texture.Unbind(this, slot);
	}

	inline void Destroy() {
		OPRENDERER_ACTIVE->Texture.Destroy(this);
	}
};

OPtexture* OPtextureLoad(const OPchar* asset);
