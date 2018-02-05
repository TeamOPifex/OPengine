#pragma once

#include "OPimage.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Core/include/OPtypes.h"
#include "./Math/include/OPvec4.h"
#include "OPtexture.h"

//-----------------------------------------------------------------------------
//   _____ _                   _
//  / ____| |                 | |
// | (___ | |_ _ __ _   _  ___| |_ ___
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/

struct OPtextureCubeOLD {
    ui16 Width, Height;
	OPtexture Texture;
};

enum struct OPtextureCubeFace {
	POSITIVE_X = 0,
	NEGATIVE_X,
	POSITIVE_Y,
	NEGATIVE_Y,
	POSITIVE_Z,
	NEGATIVE_Z
};

#include "./Human/include/Rendering/OPrender.h"
struct OPtextureCube {
	void* internalPtr;

	inline OPtextureCube* Init(OPtextureDesc desc) {
		return OPRENDERER_ACTIVE->TextureCube.Init(this, desc);
	}

	inline OPtextureCube* Init(OPimage* images) {
		return OPRENDERER_ACTIVE->TextureCube.Init(this, images);
	}

	inline OPtextureCube* Init(const OPchar** images) {
		return OPRENDERER_ACTIVE->TextureCube.Init(this, images);
	}

	inline static OPtextureCube* Create(OPimage* images) {
		return OPRENDERER_ACTIVE->TextureCube.Create(images);
	}

	inline static OPtextureCube* Create(const OPchar** images) {
		return OPRENDERER_ACTIVE->TextureCube.Create(images);
	}

	inline void Bind(ui32 slot) {
		OPRENDERER_ACTIVE->TextureCube.Bind(this, slot);
	}

	inline void Bind(ui32 side, ui32 slot) {
		OPRENDERER_ACTIVE->TextureCube.BindSide(this, side, 0, slot);
	}

	inline void Bind(ui32 side, ui32 mip, ui32 slot) {
		OPRENDERER_ACTIVE->TextureCube.BindSide(this, side, mip, slot);
	}

	inline void GenMipMaps() {
		OPRENDERER_ACTIVE->TextureCube.GenMipMaps(this);
	}

	inline void Destroy() {
		OPRENDERER_ACTIVE->TextureCube.Destroy(this);
	}

	static void Process(OPtextureCube* textureCube, OPeffect* effect, OPtexture* texture, const OPchar* param, ui32 size);
	static void Process(OPtextureCube* textureCube, OPeffect* effect, OPtextureCube* texture, const OPchar* param, ui32 size);
	static void ProcessRoughness(OPtextureCube* textureCube, OPeffect* effect, OPtextureCube* texture, const OPchar* param, ui32 size);
	static void FromEquirectangular(OPtextureCube* textureCube, OPtexture* texture, ui32 size);
	static void Convolute(OPtextureCube* textureCube, OPtextureCube* texture, ui32 size);
	static void RoughnessMap(OPtextureCube* textureCube, OPtextureCube* texture, ui32 size);

};

//-----------------------------------------------------------------------------
// ______                _   _
//|  ____|              | | (_)
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

void OPtextureCubeInit(OPtextureCubeOLD *result, OPimage *faces);
OPtextureCubeOLD * OPtextureCubeCreate(OPimage * faces);
void OPtextureCubeDestroy(OPtextureCubeOLD * result);
void OPtextureCubeFree(OPtextureCubeOLD * result);
void OPtextureCubeClearActive();
ui32 OPtextureCubeBind(OPtextureCubeOLD* result);
