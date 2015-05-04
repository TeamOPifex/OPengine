#ifndef OPENGINE_PIPELINE_SPRITESYSTEM
#define OPENGINE_PIPELINE_SPRITESYSTEM

#include "./Human/include/Rendering/Sprite/OPspriteSheet.h"
#include "./Math/include/Vectors.h"
#include "./Core/include/OPtimer.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPcam.h"
#include "./Human/include/Rendering/Primitives/OPquad.h"

enum OPspriteSystemAlign {
	OPSPRITESYSTEMALIGN_CENTER,
	OPSPRITESYSTEMALIGN_BOTTOM_CENTER,
	OPSPRITESYSTEMALIGN_VERTICAL_CENTER,
	OPSPRITESYSTEMALIGN_BOTTOM_LEFT,
	OPSPRITESYSTEMALIGN_TOP_RIGHT,
};

typedef struct {
	OPvec2 Position;
	OPvec2 Scale;
	OPvec2 Center;
	OPfloat Rotation;
	OPuint CurrentSprite;
	OPint CurrentFrame;
	OPuint CurrentElapsed;
	OPint Direction;
} OPspriteSystemSprite;

typedef struct {
	OPmesh _mesh;
	OPspriteSystemSprite* SystemSprites;
	OPspriteSystemAlign Align;
	OPuint Count;
	OPuint CountMax;
	OPfloat FrameRate;
	OPsprite** Sprites;
	OPeffect* Effect;
} OPspriteSystem;

void OPspriteSystemEffectDefault(OPeffect* effect);
void OPspriteSystemInit(OPspriteSystem* system, OPsprite** sprites, OPint count, OPeffect* effect, OPspriteSystemAlign alignment);
OPspriteSystem* OPspriteSystemCreate(OPint count, OPeffect* effect, OPspriteSystemAlign alignment);
OPspriteSystemSprite* OPspriteSystemAdd(OPspriteSystem* system);
void OPspriteSystemRemove(OPspriteSystem* system, OPspriteSystemSprite* sprite);
void OPspriteSystemUpdate(OPspriteSystem* system, OPtimer* timer);
void OPspriteSystemRender(OPspriteSystem* system, OPcam* camera);

inline OPsprite* OPspriteSystemCurrent(OPspriteSystem* system, OPspriteSystemSprite* sprite) {
	return system->Sprites[sprite->CurrentSprite];
}

inline OPboundingBox3D OPspriteSystemBoundingBox3D(OPspriteSystem* system, OPspriteSystemSprite* sprite) {
	OPsprite* curr = OPspriteSystemCurrent(system, sprite);
	OPvec2 frameSize = OPspriteCurrentFrameSize(curr);
	OPboundingBox3D playerBox = {
		OPvec3Create(sprite->Position.x - frameSize.x / 4.0, sprite->Position.y, 0),
		OPvec3Create(sprite->Position.x + frameSize.x / 4.0, sprite->Position.y + frameSize.y / 2.0, 1)
	};
	return playerBox;
}

#endif