#ifndef OP_ENGINE_HUMAN_INPUT_INPUTRECORD
#define OP_ENGINE_HUMAN_INPUT_INPUTRECORD

#include "OPkeyboard.h"
#include "./Core/include/OPtimer.h"
#include "./Data/include/OPfile.h"

typedef struct {
	ui64 timeOffset;
	OPkeyboardState keyboardState;
} OPinputRecordFrame;

typedef struct {
	void** Memory;
	OPint MemorySize;
} OPinputRecordMemoryBase;

void OPinputRecordBegin(OPtimer* start, OPinputRecordMemoryBase* memoryMaps, ui16 memoryMapCount);
OPint OPinputRecordIsRunning();
OPint OPinputRecordIsRecording();
OPint OPinputRecordIsPlayingBack();
void OPinputRecordUpdate(OPtimer* timer);
void OPinputRecordPlayback();
void OPinputRecordEnd();

#endif