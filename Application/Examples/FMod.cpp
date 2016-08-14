#include "./ExampleSelectorState.h"
#include "./Pipeline/include/OPfmod.h"

#ifdef OPIFEX_OPTION_FMOD

typedef struct {
	OPfmodSound* sound[5];
} FModExample;

FModExample fmodExample;

void ExampleFModEnter(OPgameState* last) {
	OPfmodInit();
	fmodExample.sound[0] = OPfmodLoad("Audio/Pickup_Coin.wav");
	fmodExample.sound[1] = OPfmodLoad("Audio/pew.wav");
	fmodExample.sound[2] = OPfmodLoad("Audio/sfx.ogg");
	fmodExample.sound[3] = OPfmodLoad("Audio/impact.wav");
	fmodExample.sound[4] = OPfmodLoad("Audio/Powerup.wav");
}

OPint ExampleFModUpdate(OPtimer* time) {
	OPfmodUpdate();
	OPgamePad* gamePad = OPgamePadGet(OPgamePadIndex::ONE);
	if (OPkeyboardWasPressed(OPkeyboardKey::L) || gamePad->WasPressed(OPgamePadButton::A)) {
		OPfmodPlay(fmodExample.sound[0]);
	}

	if (OPkeyboardWasPressed(OPkeyboardKey::B) || gamePad->WasPressed(OPgamePadButton::B)) {
		OPfmodPlay(fmodExample.sound[1]);
	}

	if (OPkeyboardWasPressed(OPkeyboardKey::C) || gamePad->WasPressed(OPgamePadButton::B)) {
		OPfmodPlay(fmodExample.sound[2]);
	}

	if (OPkeyboardWasPressed(OPkeyboardKey::F) || gamePad->WasPressed(OPgamePadButton::B)) {
		OPfmodPlay(fmodExample.sound[3]);
	}

	if (OPkeyboardWasPressed(OPkeyboardKey::A) || gamePad->WasPressed(OPgamePadButton::B)) {
		OPfmodPlay(fmodExample.sound[4]);
	}


	return false;
}

void ExampleFModRender(OPfloat delta) {
	OPrenderClear(0, 0, 0);
	OPrenderPresent();
}

OPint ExampleFModExit(OPgameState* next) {

	return 0;
}

OPint GS_EXAMPLE_FMOD_AVAILABLE = 1;
#else
void ExampleFModEnter(OPgameState* last) {}
OPint ExampleFModUpdate(OPtimer* time) {
	return 1;
}
void ExampleFModRender(OPfloat delta) {

}
OPint ExampleFModExit(OPgameState* next) {
	return 0;
}

OPint GS_EXAMPLE_FMOD_AVAILABLE = 0;
#endif
OPgameState GS_EXAMPLE_FMOD = {
	ExampleFModEnter,
	ExampleFModUpdate,
	ExampleFModRender,
	ExampleFModExit
};
