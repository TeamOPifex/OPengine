#include "./Data/include/OPallocLinear.h"
#include "./Core/include/OPmemory.h"

void OPallocLinearInit(OPallocLinear* result, OPuint sizeInBytes) {
	result->_headerStart = result;
	result->_memStart = (void*)((size_t)result + sizeof(OPallocLinear));
	result->_size = sizeInBytes;
	OPallocLinearClear(result);
}

OPallocLinear* OPallocLinearCreate(OPuint sizeInBytes) {
	OPuint totalBytes = sizeof(OPallocLinear) + sizeInBytes;
	void* data = OPallocZero(totalBytes);
	OPallocLinear* alloc = (OPallocLinear*)data;
	OPallocLinearInit(alloc, sizeInBytes);
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