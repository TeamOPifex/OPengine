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
	void (*Update)(OPtimer*);
	void (*OnExit)(OPgameState*);
};

extern OPgameState* ActiveState;

void OPgameStateSwap(OPgameState* targetState); 
OPgameState* OPgameStateCreate(void (*entrance)(OPgameState*), void (*update)(OPtimer*), void (*exit)(OPgameState*));
OPint OPgameStateDestroy(OPgameState* state);

#ifdef __cplusplus
}
#endif
#endif
