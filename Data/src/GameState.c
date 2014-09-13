#include "./../include/GameStates.h"

OPgameState* ActiveState = NULL;

void OPgameStateChange(OPgameState* targetState){
	OPgameState* lastState = ActiveState;
	
	if (ActiveState && ActiveState->Destroy != NULL) ActiveState->Destroy(targetState);
	ActiveState = targetState;
	if (ActiveState->Init != NULL) ActiveState->Init(lastState);
}

OPgameState* OPgameStateCreate(void (*entrance)(OPgameState*), int(*update)(OPtimer*), void (*exit)(OPgameState*)){
	OPgameState* gs = (OPgameState*)OPalloc(sizeof(OPgameState));

	gs->Init = entrance;
	gs->Update = update;
	gs->Destroy = exit;

	return gs;
}
OPint OPgameStateDestroy(OPgameState* state){
	OPfree(state);
	return 1;
}