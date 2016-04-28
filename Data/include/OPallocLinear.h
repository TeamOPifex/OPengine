#ifndef OPENGINE_DATA_OPALLOCLINEAR
#define OPENGINE_DATA_OPALLOCLINEAR

#include "./Core/include/OPallocator.h"

/* Struct definitions */
/**
 * Linear Memory Block
 */
struct OPallocatorLinear {
	OPuint _allocCount;
	OPuint _usedMemory;
	OPuint _size;
	void* _headerStart;
	void* _memStart;
	void* _currentPos;
};
typedef struct OPallocatorLinear OPallocatorLinear;

void OPallocatorLinearInit(OPallocator* result, OPuint sizeInBytes);
OPallocator* OPallocatorLinearCreate(OPuint sizeInBytes);
void OPallocatorLinearDestroy(OPallocator* alloc);

void* OPallocatorLinearAlloc(OPallocator* alloc, OPuint sizeInBytes);
void OPallocatorLinearFree(OPallocator* alloc, void* data);
void OPallocatorLinearReset(OPallocator* alloc);

inline OPuint OPallocatorLinearUsed(OPallocatorLinear* alloc) {
    return alloc->_usedMemory;
}

#endif
