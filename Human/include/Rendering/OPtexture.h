#ifndef OPENGINE_HUMAN_RENDERER_TEXTURE
#define OPENGINE_HUMAN_RENDERER_TEXTURE

#include "./Core/include/OPtypes.h"
#include "./Math/include/OPvec4.h"

#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/OPtextureDesc.h"

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

#endif
