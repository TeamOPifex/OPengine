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
	OPcmanLoad("spriteExample.opss");

	spriteExample.sprite = (OPsprite*)OPcmanGet("spriteExample/Bear");
	spriteExample.sprite2D = OPsprite2DCreate(&spriteExample.sprite, NULL);
}

OPint ExampleSpriteUpdate(OPtimer* time) {

	OPsprite2DUpdate(spriteExample.sprite2D, time); 

	OPrenderClear(0,0,0);

	OPsprite2DPrepRender(spriteExample.sprite2D);
	OPtexturePixelate();
	OPmeshRender();

	OPrenderPresent();

	return false;

}

OPint ExampleSpriteExit(OPgameState* next) {
	return 0;
}

// This is the Game State for this SpriteExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_SPRITE = {
	ExampleSpriteEnter,
	ExampleSpriteUpdate,
	ExampleSpriteExit
};