#ifndef OPENGINE_PIPELINE_TEXTURE2D
#define OPENGINE_PIPELINE_TEXTURE2D

#include "./Math/include/OPvec2.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Rendering/OPeffect.h"

typedef struct {
	OPvec2 Position;
	OPfloat Rotation;
	OPvec2 Scale;
	OPtexture* Texture;
	OPeffect* Effect;
} OPtexture2D;

EXPORT_METHOD OPtexture2D* OPtexture2DCreate(OPtexture* texture, OPeffect* effect);
EXPORT_METHOD void OPtexture2DDestroy(OPtexture2D* character);
EXPORT_METHOD void OPtexture2DPrepRender(OPtexture2D* tex2d);
EXPORT_METHOD void OPtexture2DRender(OPtexture2D* tex2d);

#endif