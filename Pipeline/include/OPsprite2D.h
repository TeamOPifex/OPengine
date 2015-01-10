#ifndef OPENGINE_PIPELINE_SPRITE2D
#define OPENGINE_PIPELINE_SPRITE2D

#include "./Human/include/Rendering/Sprite/OPspriteSheet.h"
#include "./Math/include/Vectors.h"
#include "./Core/include/OPtimer.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPcam.h"

typedef struct {
	OPvec2 Position;
	OPfloat Rotation;
	OPvec2 Scale;
	OPint Direction;
	OPsprite** Sprites;
	OPsprite* CurrentSprite;
	OPfloat FrameRate;
	OPint CurrentFrame;
	ui64 CurrentElapsed;
	OPint Loop;
	OPeffect* Effect;
} OPsprite2D;

EXPORT_METHOD void OPsprite2DInit(OPeffect* effect);
EXPORT_METHOD OPsprite2D* OPsprite2DCreate(OPsprite** sprites, OPeffect* effect);
EXPORT_METHOD void OPsprite2DDestroy(OPsprite2D* sprite);
EXPORT_METHOD void OPsprite2DUpdate(OPsprite2D* sprite, OPtimer* timer);
EXPORT_METHOD void OPsprite2DSetSprite(OPsprite2D* sprite, i32 index);
EXPORT_METHOD void OPsprite2DPrepRender(OPsprite2D* sprite);
EXPORT_METHOD void OPsprite2DRender(OPsprite2D* sprite);


#endif