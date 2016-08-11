#pragma once

#include "OPtypes.h"

struct OPallocator {
	void* data;

	void*(*alloc)(void*, OPuint);
	void(*free)(void*, void*);
	void(*clear)(void*);
};
typedef struct OPallocator OPallocator;