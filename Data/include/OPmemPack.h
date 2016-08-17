#pragma once

struct OPmemDesc;
typedef struct OPmemDesc OPmemDesc;

#include "./Core/include/OPtypes.h"

struct OPmemDesc {
	void (*Alloc)(void* segmentPtr, OPuint count);
	void (*Dealloc)(void* data);
	OPuint (*Size)(OPuint count);
	void* Data;
};

/* Allocates a block of memory to be used in segements
* @param segments descriptors
* @param segCount number of segments
* @param n size to allocate
* @return pointer to the allocated block of memory
*/
void* OPmemPackAlloc(OPmemDesc* segments, OPuint segCount, OPint n);

void OPmemPackDealloc(void* Data, OPmemDesc* segments, OPint segCount, OPint n);