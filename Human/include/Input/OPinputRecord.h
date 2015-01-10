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

EXPORT_METHOD void OPinputRecordBegin(OPtimer* start, OPinputRecordMemoryBase* memoryMaps, ui16 memoryMapCount);
EXPORT_METHOD OPint OPinputRecordIsRunning();
EXPORT_METHOD OPint OPinputRecordIsRecording();
EXPORT_METHOD OPint OPinputRecordIsPlayingBack();
EXPORT_METHOD void OPinputRecordUpdate(OPtimer* timer);
EXPORT_METHOD void OPinputRecordPlayback();
EXPORT_METHOD void OPinputRecordEnd();

#endif