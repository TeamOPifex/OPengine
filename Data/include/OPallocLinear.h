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

void OPallocatorLinearInit(OPallocatorLinear* result, OPuint sizeInBytes);
OPallocator* OPallocatorLinearCreate(OPuint sizeInBytes);
void OPallocatorLinearDestroy(OPallocatorLinear* alloc);

void* OPallocatorLinearAlloc(OPallocatorLinear* alloc, OPuint sizeInBytes);
void OPallocatorLinearFree(OPallocatorLinear* alloc, void* data);
void OPallocatorLinearReset(OPallocatorLinear* alloc);

inline OPuint OPallocatorLinearUsed(OPallocatorLinear* alloc) {
    return alloc->_usedMemory;
}

#endif
