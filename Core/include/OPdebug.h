#pragma once

struct OPtimedBlock;
typedef struct OPtimedBlock OPtimedBlock;

#include "./Core/include/OPtimer.h"
#include "./Core/include/OPlog.h"

struct OPtimedBlock {
	OPtimer timer;
	const OPchar* filename;
	const OPchar* func;
	OPuint lineNumber;

	OPtimedBlock(OPuint counter, const OPchar* filename, OPuint lineNumber, const OPchar* func) {
		timer.Init();
		this->filename = filename;
		this->lineNumber = lineNumber;
		this->func = func;
	}

	~OPtimedBlock() {
		timer.Tick();
		OPlogChannel((ui32)OPlogLevel::TIMING, "TIME", "%d ms for %s ", timer.Elapsed, func);
		OPlogChannel((ui32)OPlogLevel::TIMING, "TIMEINFO", "%s:%d", filename, lineNumber);
	}
};

#ifdef _DEBUG
	#define TIMED_BLOCK__(num) OPtimedBlock TimedBlock__##num(__COUNTER__, __FILE__, __LINE__, __FUNCTION__);
	#define TIMED_BLOCK_(num) TIMED_BLOCK__(num)
	#define TIMED_BLOCK TIMED_BLOCK_(__LINE__)
#else
	#define TIMED_BLOCK 
#endif