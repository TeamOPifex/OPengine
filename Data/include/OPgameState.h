#pragma once

struct OPgameState;
typedef struct OPgameState OPgameState;

#include "./Core/include/OPtimer.h"

struct OPgameState {
	void (*Init)(OPgameState*);
	OPint(*Update)(struct OPtimer*);
	void(*Render)(OPfloat);
	OPint (*Exit)(OPgameState*);
	void* Data;


	/* Changes the current game state, on change calls the Exit on the current game state, and then the initialize on the new game state
	* @param targetState OPgameState to change to
	*/
	static void Change(OPgameState* targetState);

	/* Creates a new OPgameState
	* @param init The Initialize function for the game state
	* @param update The Update function for the game state
	* @param exit The Exit function for the game state
	* @return A new OPgameState
	*/
	static OPgameState* Create(void(*init)(OPgameState*), OPint(*update)(struct OPtimer*), OPint(*exit)(OPgameState*));

	/* Destroys an OPgameState by deallocating the memory
	* @param state The game state to destroy
	* @return Success Result
	*/
	static OPint Destroy(OPgameState* state);
};

extern OPgameState* ActiveState;