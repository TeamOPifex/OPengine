#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./OPengine.h"
#include "./Pipeline/include/OPspriteSystem.h"

typedef struct {
	OPspriteSystem spriteSystem;
	OPeffect spriteEffect;
	OPsprite* sprites[2];
	OPcam camera;
} SpriteSystemExample;

SpriteSystemExample spriteSystemExample;

void ExampleSpriteSystemEnter(OPgameState* last) {

	OPCMAN.Load("spriteExample.opss");

	spriteSystemExample.sprites[0] = (OPsprite*)OPCMAN.Get("spriteExample/Bear");
	OPspriteSystemEffectDefault(&spriteSystemExample.spriteEffect);
	OPspriteSystemInit(&spriteSystemExample.spriteSystem, spriteSystemExample.sprites, 5, &spriteSystemExample.spriteEffect, OPSPRITESYSTEMALIGN_CENTER);
	OPspriteSystemSprite* sp = OPspriteSystemAdd(&spriteSystemExample.spriteSystem);
	sp->Scale = OPvec2(10, 10);
	sp->Position = OPvec2(0, 0);

    OPfloat halfWidth = (OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / 2.0;
    OPfloat halfHeight = (OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height / 2.0;
	spriteSystemExample.camera.SetOrtho(OPvec3Create(0, 0, -1), OPvec3(0, 0, 0), OPvec3(0, 1, 0), 0.1f, 10.0f, -halfWidth, halfWidth, -halfHeight, halfHeight);
}

OPint ExampleSpriteSystemUpdate(OPtimer* time) {

	OPspriteSystemUpdate(&spriteSystemExample.spriteSystem, time);

    OPrenderCull(false);
	OPrenderClear(0.3);

	OPspriteSystemRender(&spriteSystemExample.spriteSystem, &spriteSystemExample.camera);

	OPrenderPresent();

	return false;

}
void ExampleSpriteSystemRender(OPfloat delta) {

}
OPint ExampleSpriteSystemExit(OPgameState* next) {
	return 0;
}

OPint GS_EXAMPLE_SPRITESYSTEM_AVAILABLE = 1;
// This is the Game State for this SpriteSystemExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_SPRITESYSTEM = {
	ExampleSpriteSystemEnter,
	ExampleSpriteSystemUpdate,
	ExampleSpriteSystemRender,
	ExampleSpriteSystemExit
};
