#include "./Data/include/OPallocLinear.h"
#include "./Core/include/OPmemory.h"

OPallocLinear* OPallocLinearCreate(OPuint sizeInBytes) {
	ui64 totalBytes = sizeof(OPallocLinear) + sizeInBytes;
	void* data = OPalloc(totalBytes);
	OPallocLinear* alloc = (OPallocLinear*)data;
	alloc->_headerStart = data;
	alloc->_memStart = (void*)((OPint)data + sizeof(OPallocLinear));
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
	alloc->_currentPos = (void*)((OPint)block + sizeInBytes);
	alloc->_usedMemory += sizeInBytes;
	alloc->_allocCount++;
	return block;
}