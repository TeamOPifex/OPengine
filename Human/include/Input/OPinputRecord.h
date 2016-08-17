#pragma once

struct OPinputRecordFrame;
struct OPinputRecordMemoryBase;

typedef struct OPinputRecordFrame OPinputRecordFrame;
typedef struct OPinputRecordMemoryBase OPinputRecordMemoryBase;

#include "./Human/include/Input/OPkeyboard.h"
#include "./Core/include/OPtimer.h"
#include "./Data/include/OPfile.h"

struct OPinputRecordFrame {
	ui64 timeOffset;
	OPkeyboard keyboardState;
};

struct OPinputRecordMemoryBase {
	void** Memory;
	OPint MemorySize;
};

void OPinputRecordBegin(OPtimer* start, OPinputRecordMemoryBase* memoryMaps, ui16 memoryMapCount);
OPint OPinputRecordIsRunning();
OPint OPinputRecordIsRecording();
OPint OPinputRecordIsPlayingBack();
void OPinputRecordUpdate(OPtimer* timer);
void OPinputRecordPlayback();
void OPinputRecordEnd();