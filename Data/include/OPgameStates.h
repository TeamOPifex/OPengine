#pragma once
#ifndef OPEngine_Data_GameStates
#define OPEngine_Data_GameStates
#include "./../../Core/include/Timer.h"
#include "./../../Core/include/Types.h"
#include "OPlist.h"

#ifdef __cplusplus
extern "C" {
#endif
struct OPgameState_def;
typedef struct OPgameState_def OPgameState;

typedef struct OPgameState_def{
	void (*OnEntrance)(OPgameState*);
	int(*Update)(OPtimer*);
	void (*OnExit)(OPgameState*);
};

extern OPgameState* ActiveState;

void OPgameStateChange(OPgameState* targetState); 
OPgameState* OPgameStateCreate(void (*entrance)(OPgameState*), int(*update)(OPtimer*), void (*exit)(OPgameState*));
OPint OPgameStateDestroy(OPgameState* state);

#ifdef __cplusplus
}
#endif
#endif
