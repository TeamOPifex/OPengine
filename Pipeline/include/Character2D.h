#ifndef OPENGINE_PIPELINE_CHARACTER2D
#define OPENGINE_PIPELINE_CHARACTER2D

#include "./Human/include/Rendering/Sprite/SpriteSheet.h"
#include "./Math/include/Vectors.h"
#include "./Core/include/Timer.h"
#include "./Human/include/Rendering/Effect.h"
#include "./Human/include/Rendering/Camera.h"

typedef struct {
	OPvec2 Position;
	OPfloat Rotation;
	OPvec2 Scale;
	OPint Direction;
	OPsprite** Sprites;
	OPsprite* CurrentSprite;
	OPfloat FrameRate;
	OPint Loop;
	OPeffect* Effect;
} OPcharacter2D;

void OPcharacter2DInit(OPeffect* effect);
OPcharacter2D* OPcharacter2DCreate(OPsprite** sprites, OPeffect* effect);
void OPcharacter2DDestroy(OPcharacter2D* character);
void OPcharacter2DUpdate(OPcharacter2D* character, OPtimer* timer);
void OPcharacter2DSetSprite(OPcharacter2D* character, i32 sprite);
void OPcharacter2DRender(OPcharacter2D* character);


#endif