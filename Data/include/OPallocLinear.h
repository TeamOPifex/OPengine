#ifndef OPENGINE_DATA_OPALLOCLINEAR
#define OPENGINE_DATA_OPALLOCLINEAR

#include "./Core/include/OPtypes.h"

typedef struct {
	OPuint _allocCount;
	OPuint _usedMemory;
	OPuint _size;
	void* _headerStart;
	void* _memStart;
	void* _currentPos;
} OPallocLinear;

EXPORT_METHOD OPallocLinear* OPallocLinearCreate(OPuint sizeInBytes);
EXPORT_METHOD void OPallocLinearDestroy(OPallocLinear* alloc);
EXPORT_METHOD void* OPallocLinearAlloc(OPallocLinear* alloc, OPuint sizeInBytes);

inline void OPallocLinearClear(OPallocLinear* alloc) {
	alloc->_allocCount = 0;
	alloc->_usedMemory = 0;
	alloc->_currentPos = alloc->_memStart;
}

#endif