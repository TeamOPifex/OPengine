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

	OPcmanLoad("spriteExample.opss");

	spriteSystemExample.sprites[0] = (OPsprite*)OPcmanGet("spriteExample/Bear");
	OPspriteSystemEffectDefault(&spriteSystemExample.spriteEffect);
	OPspriteSystemInit(&spriteSystemExample.spriteSystem, spriteSystemExample.sprites, 5, &spriteSystemExample.spriteEffect, OPSPRITESYSTEMALIGN_BOTTOM_CENTER);
	OPspriteSystemSprite* sp = OPspriteSystemAdd(&spriteSystemExample.spriteSystem);
	sp->Scale = OPvec2(10, 10);
	sp->Position = OPvec2(300, 100);

	spriteSystemExample.camera = OPcamOrtho(OPvec3Create(0, 0, 10), OPVEC3_ZERO, OPVEC3_UP, 0.1f, 20.0f, 0, (OPfloat)OPRENDER_WIDTH, 0, (OPfloat)OPRENDER_HEIGHT);
}

OPint ExampleSpriteSystemUpdate(OPtimer* time) {

	OPspriteSystemUpdate(&spriteSystemExample.spriteSystem, time);

	OPrenderClear(0, 0, 0);

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
