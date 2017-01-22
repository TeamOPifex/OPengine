#include "./ExampleSelectorState.h"
#include "./Pipeline/include/OPfmod.h"


class FModExample : public OPgameState {
#ifdef OPIFEX_OPTION_FMOD
	OPfmodSound* sound[5];
#endif


	void Init(OPgameState* last) {
#ifdef OPIFEX_OPTION_FMOD
		OPfmodInit();
		sound[0] = OPfmodLoad("Audio/Pickup_Coin.wav");
		sound[1] = OPfmodLoad("Audio/pew.wav");
		sound[2] = OPfmodLoad("Audio/sfx.ogg");
		sound[3] = OPfmodLoad("Audio/impact.wav");
		sound[4] = OPfmodLoad("Audio/Powerup.wav");
#endif
	}

	OPint Update(OPtimer* time) {

#ifdef OPIFEX_OPTION_FMOD
		OPfmodUpdate();
		OPgamePad* gamePad = OPGAMEPADS[0];
		if (OPKEYBOARD.WasPressed(OPkeyboardKey::L) || gamePad->WasPressed(OPgamePadButton::A)) {
			OPfmodPlay(sound[0]);
		}

		if (OPKEYBOARD.WasPressed(OPkeyboardKey::B) || gamePad->WasPressed(OPgamePadButton::B)) {
			OPfmodPlay(sound[1]);
		}

		if (OPKEYBOARD.WasPressed(OPkeyboardKey::C) || gamePad->WasPressed(OPgamePadButton::B)) {
			OPfmodPlay(sound[2]);
		}

		if (OPKEYBOARD.WasPressed(OPkeyboardKey::F) || gamePad->WasPressed(OPgamePadButton::B)) {
			OPfmodPlay(sound[3]);
		}

		if (OPKEYBOARD.WasPressed(OPkeyboardKey::A) || gamePad->WasPressed(OPgamePadButton::B)) {
			OPfmodPlay(sound[4]);
		}
#endif

		return false;
	}

	void Render(OPfloat delta) {
		OPrenderClear(0, 0, 0);
		OPrenderPresent();
	}

	OPint Exit(OPgameState* next) {

		return 0;
	}
};



#ifdef OPIFEX_OPTION_FMOD
OPint GS_EXAMPLE_FMOD_AVAILABLE = 1;
#else
OPint GS_EXAMPLE_FMOD_AVAILABLE = 0;
#endif
FModExample _GS_EXAMPLE_FMOD;
OPgameState* GS_EXAMPLE_FMOD = &_GS_EXAMPLE_FMOD;