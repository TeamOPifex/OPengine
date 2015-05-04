#include "./Human/include/Audio/OPaudioPlayer.h"


OPaudioPlayer* OPAUD_CURR_PLAYER;

OPaudioPlayer OPaudPlayerCreate(OPaudioSource* src, OPint sounds, OPint looping){
	OPaudioPlayer player = {
		NULL,
		sounds,
		0
	};

#ifdef OPIFEX_OPTION_AUDIO
	OPlog("Sounds: %d\n", sounds);
	player.Emitters = (OPaudioEmitter**)OPalloc(sizeof(OPaudioEmitter*) * sounds);

	for(OPint i = sounds; i--;){
		player.Emitters[i] = OPaudCreateEmitter(
			src,
			EMITTER_THREADED | (looping ? EMITTER_LOOPING : 0x0)
		);
	}

#endif
	return player;
}
//-----------------------------------------------------------------------------
void OPaudPlayerVolume(OPfloat gain){
#ifdef OPIFEX_OPTION_AUDIO
	OPmutexLock(&OPAUD_CURR_MUTEX);
	for(OPint i = OPAUD_CURR_PLAYER->Count; i--;){
		OPaudVolume(OPAUD_CURR_PLAYER->Emitters[i], gain);
	}
	OPmutexUnlock(&OPAUD_CURR_MUTEX);
#endif
}
//-----------------------------------------------------------------------------
void OPaudPlayerDestroy(OPaudioPlayer* player){
#ifdef OPIFEX_OPTION_AUDIO
	OPfree(player->Emitters);
#endif
}
//-----------------------------------------------------------------------------
void OPaudPlayerPlay(){
#ifdef OPIFEX_OPTION_AUDIO
	OPaudSafePlay(OPAUD_CURR_PLAYER->Emitters[OPAUD_CURR_PLAYER->Current++]);
	OPAUD_CURR_PLAYER->Current %= OPAUD_CURR_PLAYER->Count;
#endif
}
//-----------------------------------------------------------------------------
void OPaudPlayerStop(){
#ifdef OPIFEX_OPTION_AUDIO
	for(OPint i = OPAUD_CURR_PLAYER->Count; i--;){
		if((*OPAUD_CURR_PLAYER->Emitters[i]).State != Stopped){
			OPaudSafeStop(OPAUD_CURR_PLAYER->Emitters[i]);
		}
	}
#endif
}
//-----------------------------------------------------------------------------
void OPaudPlayerPause(){
#ifdef OPIFEX_OPTION_AUDIO
	for(OPint i = OPAUD_CURR_PLAYER->Count; i--;){
		if((*OPAUD_CURR_PLAYER->Emitters[i]).State != Paused){
			OPaudSafePause(OPAUD_CURR_PLAYER->Emitters[i]);
		}
	}
#endif
}
//-----------------------------------------------------------------------------
void OPaudPlayerUpdate(void(*Proc)(OPaudioEmitter* emit, OPint length)){
#ifdef OPIFEX_OPTION_AUDIO
	for(OPint i = OPAUD_CURR_PLAYER->Count; i--;){
		OPaudSafeUpdate(OPAUD_CURR_PLAYER->Emitters[i], Proc);
	}
#endif
}