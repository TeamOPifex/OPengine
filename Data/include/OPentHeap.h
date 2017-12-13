#pragma once

struct OPentHeap;
typedef struct OPentHeap OPentHeap;

#include "./Data/include/OPminHeap.h"
#include "./Core/include/OPmemory.h"
#include <stdio.h>

struct OPentHeap {
	void*   Entities;
	OPint*  InUse;
	OPint   EntSize;
	OPint   MaxIndex;
	OPuint* Size;
	OPuint  Capacity;
	OPminHeap Free;

	OPentHeap() { }
	OPentHeap(void* segPtr, OPint entSize, OPint count) { Init(segPtr, entSize, count); }

	void Init(void* segPtr, OPint entSize, OPint count);
	void Destroy();

	/* Fills the pointer i with the index of the activated entity in the EntHeap
	* @param i Pointer to an OPint, it will contain the activated entity
	*/
	void Activate(OPint* i);

	/* Kill off an entity in an EntHeap. It will be re-used when OPentHeapActivate is called.
	* @param heap The EntHeap to kill of an entity
	* @param i Index of the entity in the EntHeap
	*/
	void Kill(OPint i);

	/* Gets the size required to create an EntHeap
	* @param entsize Size of each entity needed for the EntHeap
	* @param count Number of entities to manage
	* @return size in bytes required
	*/
	static OPuint Bytes(OPint entsize, OPint count);
	
	/* Get an indexed entity out of an EntHeap
	* @param heap The EntHeap to get an entity
	* @param i The index in the EntHeap to get
	* @return Pointer to the entity
	*/
	inline void* Get(OPint i) {
		return ((ui8*)Entities) + (i * EntSize);
	}

	/* Creates an EntHeap
	* @param segPtr Pointer to a block of memory, use OPentHeapBytes to alloc the right size
	* @param entSize Size of each entity in the EntHeap
	* @param count Number of entities to manage
	* @return A new EntHeap
	*/
	inline static OPentHeap* Create(void* segPtr, OPint entSize, OPint count) {
		return OPNEW(OPentHeap(segPtr, entSize, count));
	}
};

#define OPentHeapIsLiving(heap, i) if(!heap->InUse[i]) continue;