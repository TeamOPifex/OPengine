#pragma once

class OPgameState;

#include "./Core/include/OPtimer.h"

class OPgameState {
public:
	virtual void Init(OPgameState*) = 0;
	virtual OPint Update(struct OPtimer*) = 0;
	virtual void Render(OPfloat) = 0;
	virtual OPint Exit(OPgameState*) = 0;
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
	//static OPgameState* Create(void(*init)(OPgameState*), OPint(*update)(struct OPtimer*), OPint(*exit)(OPgameState*));

	/* Destroys an OPgameState by deallocating the memory
	* @param state The game state to destroy
	* @return Success Result
	*/
	static OPint Destroy(OPgameState* state);
};

extern OPgameState* ActiveState;