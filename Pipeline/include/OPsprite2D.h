#pragma once

#include "./Human/include/Rendering/Sprite/OPspriteSheet.h"
#include "./Math/include/Vectors.h"
#include "./Core/include/OPtimer.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPcam.h"

struct OPsprite2D {
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
};

void OPsprite2DInit(OPeffect* effect);
OPsprite2D* OPsprite2DCreate(OPsprite** sprites, OPeffect* effect);
void OPsprite2DDestroy(OPsprite2D* sprite);
void OPsprite2DUpdate(OPsprite2D* sprite, OPtimer* timer);
void OPsprite2DSetSprite(OPsprite2D* sprite, i32 index);
void OPsprite2DPrepRender(OPsprite2D* sprite);
void OPsprite2DRender(OPsprite2D* sprite);