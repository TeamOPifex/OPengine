#include "./Data/include/OPallocLinear.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/Assert.h"

void OPallocatorLinearInit(OPallocatorLinear* result, OPuint sizeInBytes) {
	result->_headerStart = result;
	result->_memStart = (void*)((size_t)result + sizeof(OPallocatorLinear));
	result->_size = sizeInBytes;
	OPallocatorLinearReset(result);
}

OPallocator* OPallocatorLinearCreate(OPuint sizeInBytes) {
	OPallocator* allocator = (OPallocator*)OPalloc(sizeof(OPallocator));

	OPuint totalBytes = sizeof(OPallocatorLinear) + sizeInBytes;
	void* data = OPallocZero(totalBytes);

	OPallocatorLinear* alloc = (OPallocatorLinear*)data;
	OPallocatorLinearInit(alloc, sizeInBytes);

	allocator->data = alloc;
	allocator->alloc = (void*(*)(void*, OPuint))OPallocatorLinearAlloc;
	allocator->free = (void(*)(void*, void*))OPallocatorLinearFree;
	allocator->clear = (void(*)(void*))OPallocatorLinearReset;
	
	return allocator;
}

void OPallocatorLinearFree(OPallocatorLinear* alloc, void* data) {

}

void OPallocatorLinearReset(OPallocatorLinear* alloc) {
	alloc->_allocCount = 0;
	alloc->_usedMemory = 0;
	alloc->_currentPos = alloc->_memStart;
}

void OPallocatorLinearDestroy(OPallocatorLinear* alloc) {
	OPfree(alloc->_headerStart);
}

// TODO: Align Memory into DWORD sized chunks
void* OPallocatorLinearAlloc(OPallocatorLinear* alloc, OPuint sizeInBytes) {
	//OPallocAlignAdjustment
	if(alloc->_usedMemory + sizeInBytes > alloc->_size) {
	    ASSERT(false, "NO MEMORY LEFT");
		return NULL;
	}
	void* block = alloc->_currentPos;
	alloc->_currentPos = (void*)((size_t)block + sizeInBytes);
	alloc->_usedMemory += sizeInBytes;
	alloc->_allocCount++;
	return block;
}
