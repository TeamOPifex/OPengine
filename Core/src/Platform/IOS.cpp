#include "./Core/include/OPcore.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPgameCycle.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/Assert.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPdir.h"

#if defined(OPIFEX_IOS)
void OPstartRender() {
	OPrender(1.0f);
}

OPint OPstartUpdate() {
    if(!_OPengineRunning) return 0;

   	// update the timer
	OPtimerTick(&OPtime);

	// update the game
	if (OPupdate(&OPtime)) {
		OPENGINERUNNING = false;

		// game loop has finished, clean up
    	OPdestroy();

    	OPfree(OPSTARTUP_PATH);
    	OPfree(OPEXECUTABLE_PATH);

    	#ifndef OPIFEX_OPTION_RELEASE
    		OPlog("Alloc/Dealloc/Diff: %d / %d / %d", OPallocations, OPdeallocations, (OPallocations - OPdeallocations));
    		ASSERT((OPallocations - OPdeallocations) == 0, "ALERT - Not all allocated memory was freed");
    	#endif

		return 1;
	}
}

void OPstart(int argc, char** args) {
	OPSTARTUP_PATH = OPdirCurrent();
	OPEXECUTABLE_PATH = OPdirExecutable();
	OPtime.Init();
	OPENGINERUNNING = true;
	OPinitialize();
}
#endif