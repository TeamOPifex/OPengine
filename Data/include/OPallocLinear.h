#ifndef OPENGINE_DATA_OPALLOCLINEAR
#define OPENGINE_DATA_OPALLOCLINEAR

#include "./Core/include/OPtypes.h"

struct OPallocLinear {
	OPuint _allocCount;
	OPuint _usedMemory;
	OPuint _size;
	void* _headerStart;
	void* _memStart;
	void* _currentPos;
};
typedef struct OPallocLinear OPallocLinear;

OPallocLinear* OPallocLinearCreate(OPuint sizeInBytes);
void OPallocLinearDestroy(OPallocLinear* alloc);
void* OPallocLinearAlloc(OPallocLinear* alloc, OPuint sizeInBytes);

inline void OPallocLinearClear(OPallocLinear* alloc) {
	alloc->_allocCount = 0;
	alloc->_usedMemory = 0;
	alloc->_currentPos = alloc->_memStart;
}

#endif
