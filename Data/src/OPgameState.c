#include "./Data/include/OPgameState.h"

OPgameState* ActiveState = NULL;

void OPgameStateChange(OPgameState* targetState){
	
	if (ActiveState && ActiveState->Exit != NULL) { 
		if(ActiveState->Exit(targetState)) {
			return;
		}
	}
	OPgameState* lastState = ActiveState;
	ActiveState = targetState;
	if (ActiveState->Init != NULL) ActiveState->Init(lastState);
}

OPgameState* OPgameStateCreate(void (*init)(OPgameState*), OPint(*update)(OPtimer*), OPint(*exit)(OPgameState*)){
	OPgameState* gs = (OPgameState*)OPalloc(sizeof(OPgameState));

	gs->Init = init;
	gs->Update = update;
	gs->Exit = exit;
	gs->Data = NULL;

	return gs;
}

OPint OPgameStateDestroy(OPgameState* state){
	OPfree(state);
	return 1;
}