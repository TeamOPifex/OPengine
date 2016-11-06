#include "./Core/include/OPcore.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPgameCycle.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/Assert.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPdir.h"

#ifdef OPIFEX_WINDOWS
	HINSTANCE OP_HINSTANCE;
#endif

#ifdef OPIFEX_DESKTOP
void OPstart(int argc, char** args) {
	// Initialize the engine and game
	OPSTARTUP_PATH = OPdirCurrent();
	OPEXECUTABLE_PATH = OPdirExecutable();
    OPlogErr("Exec Path: %s", OPEXECUTABLE_PATH);
	OPTIMER.Init();
	OPENGINERUNNING = true;
	OPinitialize();

	// main game loop
	while(OPENGINERUNNING){
		// update the timer
		OPTIMER.Tick();

		// Make sure that at least 1 ms has passed
		if (OPTIMER.Elapsed == 0) continue;

		// update the game
		if (OPupdate(&OPTIMER)) {
			OPENGINERUNNING = false;
			break;
		}
		OPrender(1.0f);
	}

	// game loop has finished, clean up
	OPdestroy();

	OPfree(OPSTARTUP_PATH);
	OPfree(OPEXECUTABLE_PATH);

	#ifndef OPIFEX_OPTION_RELEASE
	OPlogDebug("Alloc/Dealloc/Diff: %d / %d / %d", OPallocations, OPdeallocations, (OPallocations - OPdeallocations));
	ASSERT((OPallocations - OPdeallocations) == 0, "ALERT - Not all allocated memory was freed");
	#endif
}


void OPstartStepped(int argc, char** args) {
	OPtimer frameStepped;
	ui64 accumlator = 0;
	ui64 STEP = 16;

	// Initialize the engine and game
	OPSTARTUP_PATH = OPdirCurrent();
	OPEXECUTABLE_PATH = OPdirExecutable();

	OPTIMER.Init();
	frameStepped.Init();
	frameStepped.Elapsed = STEP;

	OPENGINERUNNING = true;
	OPinitialize();

	// main game loop
	while (OPENGINERUNNING) {
		// update the timer
		OPTIMER.Tick();

#if _DEBUG
		// This will usually only happen if we stopped at a breakpoint
		// and then resumed. This will make sure that we don't try to
		// update 15+ seconds at a time.
		if (OPTIMER.Elapsed > 2000) {
			OPTIMER.Elapsed = 16;
		}
#endif

		accumlator += OPTIMER.Elapsed;

		while (accumlator >= STEP) {
			frameStepped.TotalGametime += STEP;
			// The Elapsed time is always set to the STEP
			// at initialization of the OPtimer
			// The TotalGameTime is incremented by STEP
			if (OPupdate(&frameStepped)) {
				OPENGINERUNNING = false;
				break;
			}
			accumlator -= STEP;
		}

		if (OPENGINERUNNING) {
			OPrender(accumlator / (OPfloat)STEP);
		}
	}

	// game loop has finished, clean up
	OPdestroy();

	OPfree(OPSTARTUP_PATH);
	OPfree(OPEXECUTABLE_PATH);

#ifndef OPIFEX_OPTION_RELEASE
    OPlogChannel((ui32)OPlogLevel::MEMORY, "MEMORY", "Allocations: %d", OPallocations);
    OPlogChannel((ui32)OPlogLevel::MEMORY, "MEMORY", "Deallocations: %d", OPdeallocations);
    OPlogChannel((ui32)OPlogLevel::MEMORY, "MEMORY", "Alloc - Dealloc: %d", (OPallocations - OPdeallocations));

    OPlogChannel((ui32)OPlogLevel::MEMORY, "MEMORY", "Bytes Requested: %d (may not be correct because of realloc)", OPallocationBytesRequested);
    OPlogChannel((ui32)OPlogLevel::MEMORY, "MEMORY", "Bytes Actual: %d", OPallocationBytes);
    OPlogChannel((ui32)OPlogLevel::MEMORY, "MEMORY", "Bytes of Header: %d", OPallocationBytes - OPallocationBytesRequested);
    OPlogChannel((ui32)OPlogLevel::MEMORY, "MEMORY", "Deallocated Bytes: %d", OPdeallocationBytes);
    OPlogChannel((ui32)OPlogLevel::MEMORY, "MEMORY", "Alloc - Dealloc Bytes: %d", (OPallocationBytes - OPdeallocationBytes));

    ASSERT((OPallocations - OPdeallocations) == 0, "ALERT - Not all allocated memory was freed");
#endif
}
#endif
