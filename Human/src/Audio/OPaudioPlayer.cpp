#include "./Human/include/Audio/OPaudioPlayer.h"

OPaudioPlayer* OPAUD_CURR_PLAYER;

OPaudioPlayer OPaudPlayerCreate(OPaudioSource* src, OPint sounds, OPint looping){
	OPaudioPlayer player = {
		NULL,
		sounds,
		0
	};

	OPLog("Sounds: %d\n", sounds);
	player.Emitters = (OPaudioEmitter*)OPalloc(sizeof(OPaudioEmitter) * sounds);

	for(OPint i = sounds; i--;){
		player.Emitters[i] = OPaudCreateEmitter(src, looping);
	}

	return player;
}

void OPaudPlayerDestroy(OPaudioPlayer* player){
	OPfree(player->Emitters);
}

void OPaudPlayerPlay(){
	OPaudSetEmitter(&OPAUD_CURR_PLAYER->Emitters[OPAUD_CURR_PLAYER->Current++]);
	OPaudPlay();
	OPAUD_CURR_PLAYER->Current %= OPAUD_CURR_PLAYER->Count;
}

void OPaudPlayerStop(){
	for(OPint i = OPAUD_CURR_PLAYER->Count; i--;){
		if(OPAUD_CURR_PLAYER->Emitters[i].State != Stopped){
			OPaudSetEmitter(&OPAUD_CURR_PLAYER->Emitters[i]);
			OPaudStop();
		}
	}
}

void OPaudPlayerPause(){
	for(OPint i = OPAUD_CURR_PLAYER->Count; i--;){
		if(OPAUD_CURR_PLAYER->Emitters[i].State != Paused){
			OPaudSetEmitter(&OPAUD_CURR_PLAYER->Emitters[i]);
			OPaudPause();
		}
	}
}

void OPaudPlayerUpdate(void(*Proc)(OPaudioEmitter* emit, OPint length)){
	for(OPint i = OPAUD_CURR_PLAYER->Count; i--;){
		OPaudSetEmitter(&OPAUD_CURR_PLAYER->Emitters[i]);
		OPaudUpdate(Proc);
	}
}