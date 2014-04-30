#ifndef OPENGINE_PIPELINE_TEXTURE2D
#define OPENGINE_PIPELINE_TEXTURE2D

#include "./Math/include/Vector2.h"
#include "./Human/include/Rendering/Texture.h"
#include "./Human/include/Rendering/Effect.h"

typedef struct {
	OPvec2 Position;
	OPfloat Rotation;
	OPvec2 Scale;
	OPtexture* Texture;
	OPeffect* Effect;
} OPtexture2D;

OPtexture2D* OPtexture2DCreate(OPtexture* texture, OPeffect* effect);
void OPtexture2DDestroy(OPtexture2D* character);
void OPtexture2DPrepRender(OPtexture2D* tex2d);
void OPtexture2DRender(OPtexture2D* tex2d);

#endif