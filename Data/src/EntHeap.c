#include "./Data/include/EntHeap.h"

OPuint OPentHeapSize(OPint entsize, OPint count){
	OPuint size = sizeof(OPentHeap);

	// account for entities and indicies
	size += (sizeof(OPint) + entsize) * (count + 1);

	// account for list of indices in use
	size += sizeof(OPint) * count;

	return size;
}
//-----------------------------------------------------------------------------
OPentHeap* OPentHeapCreate(void* segPtr, OPint entSize, OPint count){
	OPuint off = 0, i = count, j = 0;
	OPminHeap free = {
		NULL,
		count,
		0
	};
	OPentHeap* heapPtr = NULL;
	OPentHeap heap = {
		NULL,
		NULL,
		entSize,
		0,
		{0}
	};

	heap.Free = free;

	// leave space for entity data
	off += entSize * count;

	// copy 0's into all the indices of the index list
	heap.InUse = (OPint*)((ui8*)segPtr + off);
	OPbzero(heap.InUse, sizeof(OPint) * count);
	off += sizeof(OPint) * count;

	// create the heap
	heap.Free._indices = (OPint*)((ui8*)segPtr + off);
	OPbzero(heap.Free._indices, sizeof(OPint) * (count + 1));
	off += sizeof(OPint) * (count + 1); // account for indices
	for(;i--;){
		OPminHeapPush(&heap.Free, i);
	}

	// create the ent heap
	heapPtr = (OPentHeap*)((ui8*)segPtr + off);
	heap.Entities = segPtr;
	OPbzero(heap.Entities, entSize * count);
	// copy the heap into the memory segment
	OPmemcpy(heapPtr, &heap, sizeof(OPentHeap));
	OPlog("Free._indices %x", heapPtr->Free._indices);
	OPlog("%x->Free._indices %x", heapPtr, heapPtr->Free._indices);

	OPlog("OPint Size: %d", sizeof(OPint));
	OPlog("HEAP: %x", heapPtr);
	OPlog("Entities: %x", &heapPtr->Entities);
	OPlog("InUse: %x", &heapPtr->InUse);
	OPlog("EntSize: %x", &heapPtr->EntSize);
	OPlog("MaxIndex: %x", &heapPtr->MaxIndex);
	OPlog("Free: %x", heapPtr->Free);

	return heapPtr;
}