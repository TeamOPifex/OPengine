#ifndef OPEngine_Data_GameStates
#define OPEngine_Data_GameStates
#include "./Core/include/OPtimer.h"
#include "./Core/include/OPtypes.h"
#include "./Data/include/OPlist.h"

struct OPgameState;
typedef struct OPgameState OPgameState;

struct OPgameState {
	void (*Init)(OPgameState*);
	OPint(*Update)(struct OPtimer*);
	void(*Render)(OPfloat);
	OPint (*Exit)(OPgameState*);
	void* Data;
};

extern OPgameState* ActiveState;

/* Changes the current game state, on change calls the Exit on the current game state, and then the initialize on the new game state
* @param targetState OPgameState to change to
*/
void OPgameStateChange(OPgameState* targetState);

/* Creates a new OPgameState
* @param init The Initialize function for the game state
* @param update The Update function for the game state
* @param exit The Exit function for the game state
* @return A new OPgameState
*/
OPgameState* OPgameStateCreate(void (*init)(OPgameState*), OPint(*update)(struct OPtimer*), OPint(*exit)(OPgameState*));

/* Destroys an OPgameState by deallocating the memory
* @param state The game state to destroy
* @return Success Result
*/
OPint OPgameStateDestroy(OPgameState* state);

#endif
