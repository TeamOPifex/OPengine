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
} OPcharacter2D;

void OPcharacter2DInit(OPeffect* effect);
OPcharacter2D* OPcharacter2DCreate(OPsprite** sprites);
void OPcharacter2DDestroy(OPcharacter2D* character);
void OPcharacter2DUpdate(OPcharacter2D* character, OPtimer* timer);
void OPcharacter2DSetSprite(OPcharacter2D* character, i32 sprite);
void OPcharacter2DRender(OPcharacter2D* character);


typedef struct {
	OPvec3 Position;
	OPvec3 Rotation;
	OPvec3 Scale;
	OPint Direction;
	OPsprite** Sprites;
	OPsprite* CurrentSprite;
	OPfloat FrameRate;
} OPcharacter3D;

void OPcharacter3DInit(OPeffect* effect);
OPcharacter3D* OPcharacter3DCreate(OPsprite** sprites);
void OPcharacter3DDestroy(OPcharacter3D* character);
void OPcharacter3DUpdate(OPcharacter3D* character, OPtimer* timer);
void OPcharacter3DSetSprite(OPcharacter3D* character, i32 sprite);
void OPcharacter3DRender(OPcharacter3D* character, OPcam* camera);
OPvec2 OPcharacter3DSize(OPcharacter3D* character);

#endif