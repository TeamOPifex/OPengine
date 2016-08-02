#ifndef OPENGINE_HUMAN_RENDERER_CUBEMAP
#define OPENGINE_HUMAN_RENDERER_CUBEMAP

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

#endif
