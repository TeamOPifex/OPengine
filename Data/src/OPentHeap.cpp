#include "./Data/include/OPentHeap.h"

OPuint OPentHeap::Bytes(OPint entsize, OPint count){
	OPuint size = sizeof(OPentHeap);

	// account for entities and indicies
	size += (sizeof(OPint) + entsize) * (count + 1);

	// account for list of indices in use
	size += sizeof(OPint) * count;

	return size;
}

void OPentHeap::Init(void* segPtr, OPint entSize, OPint count) {
	OPuint off = 0, i = count;
	OPminHeap free = OPminHeap(count);
	OPentHeap* heapPtr = NULL;

	Entities = NULL;
	InUse = NULL;
	EntSize = entSize;
	MaxIndex = 0;
	Size = NULL;
	Capacity = count;
	Free = free;

	// leave space for entity data
	off += entSize * count;

	// copy 0's into all the indices of the index list
	InUse = (OPint*)((ui8*)segPtr + off);
	OPbzero(InUse, sizeof(OPint) * count);
	off += sizeof(OPint) * count;

	// create the heap
	Free._indices = (OPint*)((ui8*)segPtr + off);
	OPbzero(Free._indices, sizeof(OPint) * (count + 1));
	off += sizeof(OPint) * (count + 1); // account for indices
	for(;i--;){
		Free.Push(i);
	}

	// create the ent heap
	heapPtr = (OPentHeap*)((ui8*)segPtr + off);
	Entities = segPtr;

	OPbzero(Entities, entSize * count);
	// copy the heap into the memory segment
	OPmemcpy(heapPtr, this, sizeof(OPentHeap));

	// redirect the Size pointer so that it maps to the final
	// resting place of the internal size variable
	heapPtr->Size = &(heapPtr->Free._size);
}

void OPentHeap::Activate(OPint* i) {
	if (Free._size) {
		if ((*i = Free.Pop()) >= 0) {
			InUse[*i] = 1;
			if (*i >= MaxIndex - 1) {
				MaxIndex = *i + 1;
			}
		}
	}
	else
		*i = -1;
}

void OPentHeap::Kill(OPint i) {
	OPint inUse = InUse[i];

	if (inUse) {
		OPint mi = MaxIndex - 1;
		Free.Push(i);
		InUse[i] = 0;
		while (!InUse[mi] && mi >= 0) {
			mi--;
		}
		MaxIndex = mi + 1;
	}
}