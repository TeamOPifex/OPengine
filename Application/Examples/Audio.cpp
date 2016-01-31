#include "./ExampleSelectorState.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Human/include/Systems/OPaudioSystem.h"
#include "./Human/include/Systems/OPinputSystem.h"

#include "./Data/include/OPcman.h"

typedef struct {
	OPaudioEmitter* Sound;
	OPaudioEmitter* BackgroundSound;
} AudioExample;

AudioExample* audioExample;

void ExampleAudioEnter(OPgameState* last) {
	OPcmanLoad("step0.wav");
	OPcmanLoad("background.ogg");

	OPaudInit();
	OPaudInitThread(10);

	audioExample = (AudioExample*)OPalloc(sizeof(AudioExample));

	audioExample->Sound = OPaudCreateEmitter((OPaudioSource*)OPcmanGet("step0.wav"), EMITTER_THREADED);
	audioExample->BackgroundSound = OPaudCreateEmitter((OPaudioSource*)OPcmanGet("background.ogg"), EMITTER_THREADED);
}

OPint ExampleAudioUpdate(OPtimer* time) {
	
	if (OPkeyboardWasPressed(OPKEY_P) || OPgamePadWasPressed(OPgamePadGet(OPGAMEPAD_ONE), OPGAMEPADBUTTON_A)) {
		OPaudSetEmitter(audioExample->Sound);
		OPaudVolume(audioExample->Sound, 1.0f);
		OPaudPlay();
	}
	if (OPkeyboardWasPressed(OPKEY_B) || OPgamePadWasPressed(OPgamePadGet(OPGAMEPAD_ONE), OPGAMEPADBUTTON_B)) {
		OPaudSetEmitter(audioExample->BackgroundSound);
		OPaudVolume(audioExample->BackgroundSound, 0.5f);
		OPaudPlay();
	}
	return false;
}

void ExampleAudioRender(OPfloat delta) {
	OPrenderClear(0, 0, 0);
	OPrenderPresent();
}

OPint ExampleAudioExit(OPgameState* next) {
	OPaudRecycleEmitter(audioExample->Sound);
	OPcmanUnload("step0.wav");

	OPfree(audioExample);
	return 0;
}

#ifdef OPIFEX_OPTION_AUDIO
OPint GS_EXAMPLE_AUDIO_AVAILABLE = 1;
#else
OPint GS_EXAMPLE_AUDIO_AVAILABLE = 0;
#endif
OPgameState GS_EXAMPLE_AUDIO = {
	ExampleAudioEnter,
	ExampleAudioUpdate,
	ExampleAudioRender,
	ExampleAudioExit
};
