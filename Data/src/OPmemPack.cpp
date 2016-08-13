#include "./Data/include/OPmemPack.h"

#include "./Core/include/OPmemory.h"

void* OPmemPackAlloc(OPmemDesc* segments, OPuint segCount, OPint n){
	OPuint size = 0, offset = 0, i = 0;
	void* Data = NULL;

	for(i = segCount; i--;){
		size += segments[i].Size(n);
	}

	Data = OPalloc(size);

	for(i = 0; i < segCount; i++){
		segments[i].Alloc(((ui8*)Data + offset), n);
		offset += segments[i].Size(n);
	}

	return Data;
}

void OPmemPackDealloc(void* Data, OPmemDesc* segments, OPint segCount, OPint n){
	OPint off = 0, i = 0;

	// perform any clean up needed
	for(; i < segCount; i++){
		segments[i].Dealloc(((ui8*)Data + off));
		off += segments[i].Size(n);
	}

	OPfree(Data);
}