#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./OPengine.h"


class SpriteExample : public OPgameState {
	OPsprite* sprite;
	OPsprite2D* sprite2D;


	void Init(OPgameState* last) {
		OPsprite2DInit(NULL);
		OPCMAN.Load("opss");

		sprite = (OPsprite*)OPCMAN.Get("spriteExample/Bear");
		sprite2D = OPsprite2DCreate(&sprite, NULL);
	}

	OPint Update(OPtimer* time) {

		OPsprite2DUpdate(sprite2D, time);

		OPrenderClear(0, 0, 0);

		OPsprite2DPrepRender(sprite2D);
		//OPtexturePixelate();
		OPrenderDrawBufferIndexed(0);

		OPrenderPresent();

		return false;

	}
	void Render(OPfloat delta) {

	}
	OPint Exit(OPgameState* next) {
		return 0;
	}
};


OPint GS_EXAMPLE_SPRITE_AVAILABLE = 1;
// This is the Game State for this SpriteExample
// Each entry is a function pointer for Initialize, Update, Destroy
SpriteExample _GS_EXAMPLE_SPRITE;
OPgameState* GS_EXAMPLE_SPRITE = &_GS_EXAMPLE_SPRITE;