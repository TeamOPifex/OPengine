#include "./ExampleSelectorState.h"
#include "./Pipeline/include/OPfmod.h"

typedef struct {
	OPfmod fmod;
	OPfmodSound sound[5];
} FModExample;

FModExample fmodExample;

void ExampleFModEnter(OPgameState* last) {
	OPfmodInit(&fmodExample.fmod);
	OPfmodLoad(&fmodExample.fmod, &fmodExample.sound[0], "Audio/Pickup_Coin.wav");
	OPfmodLoad(&fmodExample.fmod, &fmodExample.sound[1], "Audio/pew.wav");
	OPfmodLoad(&fmodExample.fmod, &fmodExample.sound[2], "Audio/sfx.ogg");
	OPfmodLoad(&fmodExample.fmod, &fmodExample.sound[3], "Audio/impact.wav");
	OPfmodLoad(&fmodExample.fmod, &fmodExample.sound[4], "Audio/Powerup.wav");

}

OPint ExampleFModUpdate(OPtimer* time) {
	OPfmodUpdate(&fmodExample.fmod);
	if (OPkeyboardWasPressed(OPKEY_L) || OPgamePadWasPressed(OPgamePadGet(OPGAMEPAD_ONE), OPGAMEPADBUTTON_A)) {
		OPfmodPlay(&fmodExample.fmod, &fmodExample.sound[0]);
	}

	if (OPkeyboardWasPressed(OPKEY_B) || OPgamePadWasPressed(OPgamePadGet(OPGAMEPAD_ONE), OPGAMEPADBUTTON_B)) {
		OPfmodPlay(&fmodExample.fmod, &fmodExample.sound[1]);
	}

	if (OPkeyboardWasPressed(OPKEY_C) || OPgamePadWasPressed(OPgamePadGet(OPGAMEPAD_ONE), OPGAMEPADBUTTON_B)) {
		OPfmodPlay(&fmodExample.fmod, &fmodExample.sound[2]);
	}

	if (OPkeyboardWasPressed(OPKEY_F) || OPgamePadWasPressed(OPgamePadGet(OPGAMEPAD_ONE), OPGAMEPADBUTTON_B)) {
		OPfmodPlay(&fmodExample.fmod, &fmodExample.sound[3]);
	}

	if (OPkeyboardWasPressed(OPKEY_A) || OPgamePadWasPressed(OPgamePadGet(OPGAMEPAD_ONE), OPGAMEPADBUTTON_B)) {
		OPfmodPlay(&fmodExample.fmod, &fmodExample.sound[4]);
	}

	OPrenderClear(0, 0, 0);
	OPrenderPresent();
	return false;
}

OPint ExampleFModExit(OPgameState* next) {

	return 0;
}

OPint GS_EXAMPLE_FMOD_AVAILABLE = 1;
OPgameState GS_EXAMPLE_FMOD = {
	ExampleFModEnter,
	ExampleFModUpdate,
	ExampleFModExit
};
