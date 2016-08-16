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

	inline void Destroy() {
		OPRENDERER_ACTIVE->TextureCube.Destroy(this);
	}
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
