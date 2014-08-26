#include "./Examples/Audio.h"
#include "./Human/include/Systems/RenderSystem.h"

#include "./Data/include/ContentManager.h"
#include "./Human/include/Input/GamePadSystem.h"
#include "./Human/include/Input/Input.h"
#include "./Human/include/Audio/Audio.h"
#include "./Human/include/Audio/AudioPlayer.h"

OPgameState GS_EXAMPLE_AUDIO = {
	ExampleAudioEnter,
	ExampleAudioUpdate,
	ExampleAudioExit
};

typedef struct {
	OPaudioEmitter* Sound;
	OPaudioEmitter* BackgroundSound;
} AudioExample;

AudioExample* audioExample;

void ExampleAudioEnter(OPgameState* last) {
	OPcmanLoad("impact.wav");
	OPcmanLoad("background.ogg");

	OPaudInit();
	OPaudInitThread(10);

	audioExample = (AudioExample*)OPalloc(sizeof(AudioExample));

	audioExample->Sound = OPaudCreateEmitter((OPaudioSource*)OPcmanGet("impact.wav"), EMITTER_THREADED);
	audioExample->BackgroundSound = OPaudCreateEmitter((OPaudioSource*)OPcmanGet("background.ogg"), EMITTER_THREADED);

}

int ExampleAudioUpdate(OPtimer* time) {

	OPkeyboardUpdate();
	OPgamePadSystemUpdate();
	
	if (OPkeyboardWasPressed(OPKEY_P) || OPgamePadWasPressed(OPgamePad(GamePadIndex_One), GamePad_Button_A)) {
		OPaudSetEmitter(audioExample->Sound);
		OPaudVolume(1.0f);
		OPaudPlay();
	}
	if (OPkeyboardWasPressed(OPKEY_B) || OPgamePadWasPressed(OPgamePad(GamePadIndex_One), GamePad_Button_B)) {
		OPaudSetEmitter(audioExample->BackgroundSound);
		OPaudVolume(0.5f);
		OPaudPlay();
	}

	OPrenderClear(0, 0, 0);
	OPrenderPresent();
	return false;
}

void ExampleAudioExit(OPgameState* next) {
	OPaudRecycleEmitter(audioExample->Sound);
	OPcmanUnload("impact.wav");

	OPfree(audioExample);
}
