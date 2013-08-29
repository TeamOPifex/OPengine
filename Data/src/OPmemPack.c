#include "./Data/include/OPmemPack.h"

// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
void* OPmemPackAlloc(OPmemDesc* segments, OPint segCount, OPint n){
	OPuint size = 0, offset = 0;

	for(OPint i = segCount; i--;){
		size += segments[i]->Size(n);
	}

	void* Data = OPalloc(size);

	for(OPint i = 0; i < segCount; i++){
		segments[i]->Alloc((void*)(Data + off), n);
		off += segments[i]->Size(n);
	}

	return Data;
}

OPint OPmemPackDealloc(void* Data, OPmemDesc* segments, OPint segCount, OPint n){
	OPint off = 0;

	// perform any clean up needed
	for(OPint i = 0; i < segCount; i++){
		segments[i]->Dealloc((void*)(Data + off), n);
		off += segments[i]->Size(n);
	}

	OPfree(Data);
}