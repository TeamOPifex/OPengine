#pragma once

struct OPallocator;
typedef struct OPallocator OPallocator;

#include "OPtypes.h"

struct OPallocator {
	void* internalPtr;
	void* data;

	void*(*alloc)(void*, OPuint);
	void(*free)(void*, void*);
	void(*clear)(void*);
};