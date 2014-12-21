#include "./Human/include/Input/OPinputRecord.h"
#include "./Core/include/OPlog.h"

OPfile _inputRecordFile;
OPint _inputRecordRecording = 0;
OPint _inputRecordPlayback = 0;
ui64 _timeStart = 0;
ui64 _elapsedPlayback = 0;
OPinputRecordMemoryBase* _memoryMaps = NULL;
ui16 _memoryMapCount = 0;
void* _memory = NULL;
OPint totalWritten = 0;

void OPinputRecordBegin(OPtimer* start, OPinputRecordMemoryBase* memoryMaps, ui16 memoryMapCount) {
	_inputRecordFile = OPfileOpen("input.opr");
	_inputRecordRecording = 1;
	_inputRecordPlayback = 0;
	_timeStart = start->TotalGametime;
	_elapsedPlayback = 0;
	_memoryMapCount = memoryMapCount;
	ui64 memPos = 0;
	if(_memoryMapCount > 0) {
		_memoryMaps = (OPinputRecordMemoryBase*)OPalloc(sizeof(OPinputRecordMemoryBase) * memoryMapCount);
		OPmemcpy(_memoryMaps, memoryMaps, sizeof(OPinputRecordMemoryBase) * memoryMapCount);
		for(ui16 i = 0; i < _memoryMapCount; i++) {
			memPos += _memoryMaps[i].MemorySize;
		}
		_memory = OPalloc(memPos);
		memPos = 0;
		for(ui16 i = 0; i < _memoryMapCount; i++) {
			OPmemcpy((void*)((OPuint)_memory + memPos), *(_memoryMaps[i].Memory), _memoryMaps[i].MemorySize);		
			memPos += _memoryMaps[i].MemorySize;
		}
	}
}

OPint OPinputRecordIsRunning() {
	return _inputRecordRecording || _inputRecordPlayback;
}

OPint OPinputRecordIsRecording() {
	return _inputRecordRecording;
}

OPint OPinputRecordIsPlayingBack() {
	return _inputRecordPlayback;
}

void OPinputRecordUpdate(OPtimer* timer) {
	ui64 elapsed = timer->TotalGametime - _timeStart;
	if(_inputRecordRecording) {
		OPinputRecordFrame frame = {
			elapsed,
			Keyboard
		};
		//frame.keyboardState.keys[OPKEY_W] = 3;
		//frame.keyboardState.keys[OPKEY_S] = 3;
		totalWritten += OPfileWriteBytes(&_inputRecordFile, &frame, sizeof(OPinputRecordFrame));
	} else if(_inputRecordPlayback) {
		OPinputRecordFrame frame;
		void* response = OPfileReadBytes(&_inputRecordFile, sizeof(OPinputRecordFrame));
		if(response == NULL) { // Reset the loop

			if(_memoryMaps != NULL) {
				ui64 memPos = 0;
				for(ui16 i = 0; i < _memoryMapCount; i++) {
					OPmemcpy(*(_memoryMaps[i].Memory), (void*)((OPuint)_memory + memPos), _memoryMaps[i].MemorySize);		
					memPos += _memoryMaps[i].MemorySize;
				}
			}

			OPfileSeekReset(&_inputRecordFile);
			response = OPfileReadBytes(&_inputRecordFile, sizeof(OPinputRecordFrame));
		}
		frame = *((OPinputRecordFrame*)response);
		OPfree(response);
		Keyboard = frame.keyboardState;
		OPlog("W: %d", Keyboard.keys[OPKEY_W]);
		OPlog("S: %d", Keyboard.keys[OPKEY_S]);
	}
}

void OPinputRecordPlayback() {
	if(_inputRecordRecording) {
		OPfileSeekReset(&_inputRecordFile);
		OPlog("Total Written: %d bytes", totalWritten);
		_inputRecordRecording = 0;
		_inputRecordPlayback = 1;
		if(_memoryMaps != NULL) {
			ui64 memPos = 0;
			for(ui16 i = 0; i < _memoryMapCount; i++) {
				OPlog("mempos: %d", memPos);
				OPlog("copy: %d", _memoryMaps[i].MemorySize);
				OPmemcpy(*(_memoryMaps[i].Memory), (void*)((OPuint)_memory + memPos), _memoryMaps[i].MemorySize);		
				memPos += _memoryMaps[i].MemorySize;
			}
		}
	}
}

void OPinputRecordEnd() {
	if(_inputRecordRecording || _inputRecordPlayback) {
		OPfileClose(&_inputRecordFile);
		OPfree(_memoryMaps);
		OPfree(_memory);
		_memoryMaps = NULL;
		_memory = NULL;
		_inputRecordRecording = 0;
		_inputRecordPlayback = 0;
		totalWritten = 0;
	}
}