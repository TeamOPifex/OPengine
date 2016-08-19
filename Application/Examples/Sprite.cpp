#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./OPengine.h"


typedef struct {
	OPsprite* sprite;
	OPsprite2D* sprite2D;
} SpriteExample;

SpriteExample spriteExample;

void ExampleSpriteEnter(OPgameState* last) {
	OPsprite2DInit(NULL);
	OPCMAN.Load("spriteExample.opss");

	spriteExample.sprite = (OPsprite*)OPCMAN.Get("spriteExample/Bear");
	spriteExample.sprite2D = OPsprite2DCreate(&spriteExample.sprite, NULL);
}

OPint ExampleSpriteUpdate(OPtimer* time) {

	OPsprite2DUpdate(spriteExample.sprite2D, time);

	OPrenderClear(0,0,0);

	OPsprite2DPrepRender(spriteExample.sprite2D);
	//OPtexturePixelate();
	OPrenderDrawBufferIndexed(0);

	OPrenderPresent();

	return false;

}
void ExampleSpriteRender(OPfloat delta) {

}
OPint ExampleSpriteExit(OPgameState* next) {
	return 0;
}

OPint GS_EXAMPLE_SPRITE_AVAILABLE = 1;
// This is the Game State for this SpriteExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_SPRITE = {
	ExampleSpriteEnter,
	ExampleSpriteUpdate,
	ExampleSpriteRender,
	ExampleSpriteExit
};
