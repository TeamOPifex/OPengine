#ifndef OPENGINE_PIPELINE_CHARACTER3D
#define OPENGINE_PIPELINE_CHARACTER3D

#include "./Human/include/Rendering/Sprite/SpriteSheet.h"
#include "./Math/include/Vectors.h"
#include "./Core/include/Timer.h"
#include "./Human/include/Rendering/Effect.h"
#include "./Human/include/Rendering/Camera.h"

typedef struct {
	OPvec3 Position;
	OPvec3 Rotation;
	OPvec3 Scale;
	OPint Direction;
	OPsprite** Sprites;
	OPsprite* CurrentSprite;
	OPfloat FrameRate;
	OPint Loop;
	OPeffect* Effect;
} OPcharacter3D;

void OPcharacter3DInit(OPeffect* effect);
OPcharacter3D* OPcharacter3DCreate(OPsprite** sprites, OPeffect* effect);
void OPcharacter3DDestroy(OPcharacter3D* character);
void OPcharacter3DUpdate(OPcharacter3D* character, OPtimer* timer);
void OPcharacter3DSetSprite(OPcharacter3D* character, i32 sprite);
void OPcharacter3DPrepRender(OPcharacter3D* character, OPcam* camera);
void OPcharacter3DRender(OPcharacter3D* character, OPcam* camera);
OPvec2 OPcharacter3DSize(OPcharacter3D* character);

#endif