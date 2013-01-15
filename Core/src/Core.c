#include "./../include/Core.h"

#ifndef __cplusplus
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
#else
using namespace OPEngine::Core;

void OPCore::Start(){
	// Initialize the engine and game
	OPtimer* timer = OPcreateTimer();
	_OPengineRunning = 1;
	Initialize();

	// main game loop
	while(_OPengineRunning){
		// update the timer
		OPtimerTick(timer);
		
		// update the game
		Update(timer);
	}

	// game loop has finished, clean up
	Destroy();
	OPdestroyTimer(timer);
}

void OPCore::End(){
	_OPengineRunning = 0;
}
#endif