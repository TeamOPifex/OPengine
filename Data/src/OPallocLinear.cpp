#include "./Data/include/OPallocLinear.h"
#include "./Core/include/OPmemory.h"

OPallocLinear* OPallocLinearCreate(OPuint sizeInBytes) {
	OPuint totalBytes = sizeof(OPallocLinear) + sizeInBytes;
	void* data = OPallocZero(totalBytes);
	OPallocLinear* alloc = (OPallocLinear*)data;
	alloc->_headerStart = data;
	alloc->_memStart = (void*)((size_t)data + sizeof(OPallocLinear));
	alloc->_size = sizeInBytes;
	OPallocLinearClear(alloc);
	return alloc;
}

void OPallocLinearDestroy(OPallocLinear* alloc) {
	OPfree(alloc->_headerStart);
}

// TODO: Align Memory into DWORD sized chunks
void* OPallocLinearAlloc(OPallocLinear* alloc, OPuint sizeInBytes) {
	//OPallocAlignAdjustment
	if(alloc->_usedMemory + sizeInBytes > alloc->_size) {
		return NULL;
	}
	void* block = alloc->_currentPos;
	alloc->_currentPos = (void*)((size_t)block + sizeInBytes);
	alloc->_usedMemory += sizeInBytes;
	alloc->_allocCount++;
	return block;
}