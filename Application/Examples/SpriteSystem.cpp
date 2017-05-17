#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./OPengine.h"
#include "./Pipeline/include/OPspriteSystem.h"

class SpriteSystemExample : public OPgameState {
	OPspriteSystem spriteSystem;
	OPeffect spriteEffect;
	OPsprite* sprites[2];
	OPcam camera;

	void Init(OPgameState* last) {

		OPCMAN.Load("spriteExample.opss");

		sprites[0] = (OPsprite*)OPCMAN.Get("spriteExample/Bear");
		OPspriteSystemEffectDefault(&spriteEffect);
		OPspriteSystemInit(&spriteSystem, sprites, 5, &spriteEffect, OPSPRITESYSTEMALIGN_CENTER);
		OPspriteSystemSprite* sp = OPspriteSystemAdd(&spriteSystem);
		sp->Scale = OPvec2(10, 10);
		sp->Position = OPvec2(0, 0);

		OPfloat halfWidth = (OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / 2.0;
		OPfloat halfHeight = (OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height / 2.0;
		camera.SetOrtho(OPvec3Create(0, 0, -1), OPvec3(0, 0, 0), OPvec3(0, 1, 0), 0.1f, 10.0f, -halfWidth, halfWidth, -halfHeight, halfHeight);
	}

	OPint Update(OPtimer* time) {

		OPspriteSystemUpdate(&spriteSystem, time);

		OPrenderCull(false);
		OPrenderClear(0.3);

		OPspriteSystemRender(&spriteSystem, &camera);

		OPrenderPresent();

		return false;

	}
	void Render(OPfloat delta) {

	}
	OPint Exit(OPgameState* next) {
		return 0;
	}
};


OPint GS_EXAMPLE_SPRITESYSTEM_AVAILABLE = 1;
// This is the Game State for this SpriteSystemExample
// Each entry is a function pointer for Initialize, Update, Destroy
SpriteSystemExample _GS_EXAMPLE_SPRITESYSTEM;
OPgameState* GS_EXAMPLE_SPRITESYSTEM = &_GS_EXAMPLE_SPRITESYSTEM;
