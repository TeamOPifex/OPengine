#include "./Data/include/OPentHeap.h"
OPuint OPentHeapSize(OPint entsize, OPint count){
	OPuint size = sizeof(OPentHeap);

	// account for entities and indicies
	size += (sizeof(OPint) + entsize) * (count + 1);

	return size;
}
//-----------------------------------------------------------------------------
OPentHeap* OPentHeapCreate(void* segPtr, OPint entSize, OPint count){
	OPuint off = 0, i = count + 1;
	OPminHeap free = {
		NULL,
		count + 1,
		1
	};
	OPentHeap heap = {
		NULL,
		0,
		1,
		0
	};
	heap.Free = free;

	// leave space for entity data
	off += entSize * (count + 1);

	// create the heap
	heap.Free._indices = (OPint*)((ui8*)segPtr + off);
	OPbzero(heap.Free._indices, sizeof(OPint) * (count + 1));
	off += sizeof(OPint) * (count + 1); // account for indices
	for(;i--;){
		OPminHeapPush(&heap.Free, i-1);
	}

	// create the ent heap
	heap.Entities = segPtr;
	OPbzero(heap.Entities, entSize * count);

	// copy the heap into the memory segment
	OPmemcpy(((ui8*)segPtr + off), &heap, sizeof(OPentHeap));

	return (OPentHeap*)((ui8*)segPtr + off);
}