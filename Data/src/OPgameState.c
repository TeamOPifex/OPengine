#include "./../include/OPgameStates.h"

OPgameState* ActiveState;

void OPchangeGameState(OPgameState* targetState){
	OPgameState* lastState = ActiveState;
	
	if(ActiveState->OnExit != NULL) ActiveState->OnExit(targetState);
	ActiveState = targetState;
	if(ActiveState->OnEntrance != NULL) ActiveState->OnEntrance(lastState);
}

OPgameState* OPgameStateCreate(void (*entrance)(OPgameState*), void (*update)(OPtimer*), void (*exit)(OPgameState*)){
	OPgameState* gs = (OPgameState*)OPalloc(sizeof(OPgameState));

	gs->OnEntrance = entrance;
	gs->Update = update;
	gs->OnExit = exit;

	return gs;
}
OPint OPgameStateDestroy(OPgameState* state){
	OPfree(state);
	return 1;
}