#include "./../include/Core.h"

OPint _OPengineRunning;

void OPstart(){
	// Initialize the engine and game
	OPtimer* timer = OPcreateTimer();
	_OPengineRunning = 1;
	OPinitialize();

	// main game loop
	while(_OPengineRunning){
		// update the timer
		OPtimerTick(timer);
		
		// update the game
		OPupdate(timer);
	}

	// game loop has finished, clean up
	OPdestroy();
	OPdestroyTimer(timer);
}
//----------------------------------------------------------------------------
void OPend(){
	_OPengineRunning = 0;
}
