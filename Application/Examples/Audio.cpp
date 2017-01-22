#include "./ExampleSelectorState.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Human/include/Systems/OPaudioSystem.h"
#include "./Human/include/Systems/OPinputSystem.h"

#include "./Data/include/OPcman.h"

class AudioExample : public OPgameState {
	OPaudioEmitter* Sound;
	OPaudioEmitter* BackgroundSound;


	void Init(OPgameState* last) {
		OPCMAN.Load("step0.wav");
		OPCMAN.Load("background.ogg");

		OPaudInit();
		OPaudInitThread(10);

		Sound = OPaudCreateEmitter((OPaudioSource*)OPCMAN.Get("step0.wav"), EMITTER_THREADED);
		BackgroundSound = OPaudCreateEmitter((OPaudioSource*)OPCMAN.Get("background.ogg"), EMITTER_THREADED);
	}

	OPint Update(OPtimer* time) {

		if (OPKEYBOARD.WasPressed(OPkeyboardKey::P) || OPGAMEPADS[0]->WasPressed(OPgamePadButton::A)) {
			OPaudSetEmitter(Sound);
			OPaudVolume(Sound, 1.0f);
			OPaudPlay();
		}
		if (OPKEYBOARD.WasPressed(OPkeyboardKey::B) || OPGAMEPADS[0]->WasPressed(OPgamePadButton::B)) {
			OPaudSetEmitter(BackgroundSound);
			OPaudVolume(BackgroundSound, 0.5f);
			OPaudPlay();
		}
		return false;
	}

	void Render(OPfloat delta) {
		OPrenderClear(0, 0, 0);
		OPrenderPresent();
	}

	OPint Exit(OPgameState* next) {
		OPaudRecycleEmitter(Sound);
		OPCMAN.Unload("step0.wav");
		return 0;
	}
};


#ifdef OPIFEX_OPTION_AUDIO
OPint GS_EXAMPLE_AUDIO_AVAILABLE = 1;
#else
OPint GS_EXAMPLE_AUDIO_AVAILABLE = 0;
#endif
AudioExample _GS_EXAMPLE_AUDIO;
OPgameState* GS_EXAMPLE_AUDIO = &_GS_EXAMPLE_AUDIO;