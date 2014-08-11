#ifndef OPENGINE_PIPELINE_SPRITE3D
#define OPENGINE_PIPELINE_SPRITE3D

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
} OPsprite3D;

void OPsprite3DInit(OPeffect* effect);
OPsprite3D* OPsprite3DCreate(OPsprite** sprites, OPeffect* effect);
void OPsprite3DCreateFill(OPsprite3D* sprite, OPsprite** sprites, OPeffect* effect);
void OPsprite3DDestroy(OPsprite3D* sprite);
void OPsprite3DUpdate(OPsprite3D* sprite, ui64 elapsed);
void OPsprite3DSetSprite(OPsprite3D* sprite, i32 index);
void OPsprite3DPrepRender(OPsprite3D* sprite, OPcam* camera, OPvec3 offset, OPfloat rotation);
void OPsprite3DRender(OPsprite3D* sprite, OPcam* camera);
void OPsprite3DRenderOffsetRot(OPsprite3D* sprite, OPcam* camera, OPvec3 offset, OPfloat rotation);
OPvec2 OPsprite3DSize(OPsprite3D* sprite);

#endif