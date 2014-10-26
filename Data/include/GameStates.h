#ifndef OPEngine_Data_GameStates
#define OPEngine_Data_GameStates
#include "./Core/include/Timer.h"
#include "./Core/include/Types.h"
#include "List.h"

#ifdef __cplusplus
extern "C" {
#endif
struct OPgameState_def;
typedef struct OPgameState_def OPgameState;

struct OPgameState_def{
	void (*Init)(OPgameState*);
	OPint(*Update)(OPtimer*);
	void (*Exit)(OPgameState*);
	void* Data;
};

extern OPgameState* ActiveState;

void OPgameStateChange(OPgameState* targetState); 
OPgameState* OPgameStateCreate(void (*init)(OPgameState*), OPint(*update)(OPtimer*), void (*exit)(OPgameState*));
OPint OPgameStateDestroy(OPgameState* state);

#ifdef __cplusplus
}
#endif
#endif
