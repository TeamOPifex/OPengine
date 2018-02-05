#pragma once

struct OPtexture;
typedef struct OPtexture OPtexture;

#include "./Human/include/Rendering/OPtextureDesc.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Math/include/OPvec4.h"
#include "./Data/include/OPcman.h"

struct OPtexture {
	void* internalPtr;
	OPuint guid;
	OPtextureDesc textureDesc;
#ifdef _DEBUG
	OPchar* source = NULL;
#endif

	inline void SetData(const void* pixelData) {
		OPRENDERER_ACTIVE->Texture.SetData(this, pixelData);
	}

	inline void Init(OPtextureDesc desc) {
		OPRENDERER_ACTIVE->Texture.Init(this, desc);
	}

	inline void Bind(ui32 slot) {
		OPRENDERER_ACTIVE->Texture.Bind(this, slot);
	}

	inline void Unbind(ui32 slot) {
		OPRENDERER_ACTIVE->Texture.Unbind(this, slot);
	}

	inline void Destroy() {
		OPRENDERER_ACTIVE->Texture.Destroy(this);

#ifdef _DEBUG
		if (source != NULL) {
			OPfree(source);
		}
#endif
	}

	inline static OPtexture* Load(const OPchar* asset) {
		return (OPtexture*)OPCMAN.LoadGet(asset);
	}

	static void GenerateBRDF(OPtexture* texture, ui32 size);
};

OPtexture* OPtextureLoad(const OPchar* asset);
