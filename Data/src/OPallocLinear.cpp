#include "./Data/include/OPallocLinear.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/Assert.h"

void* OPallocatorLinearAlloc(OPallocator* alloc, OPuint sizeInBytes);
void OPallocatorLinearFree(OPallocator* alloc, void* data);
void OPallocatorLinearReset(OPallocator* alloc);

void OPallocatorLinear::Destroy() {
	OPfree(_rootAlloc->data);
}

void OPallocatorLinear::Init(OPallocator* allocator, OPuint sizeInBytes) {

	_size = sizeInBytes;
	_rootAlloc = allocator;	
	_rootAlloc->internalPtr = this;
	_rootAlloc->alloc = (void*(*)(void*, OPuint))OPallocatorLinearAlloc;
	_rootAlloc->free = (void(*)(void*, void*))OPallocatorLinearFree;
	_rootAlloc->clear = (void(*)(void*))OPallocatorLinearReset;
	_rootAlloc->data = OPallocZero(_size);

	Reset();
}

void OPallocatorLinearFree(OPallocator* alloc, void* data) {

}

void OPallocatorLinearReset(OPallocator* alloc) {
	OPallocatorLinear* data = (OPallocatorLinear*)alloc->internalPtr;
	data->_allocCount = 0;
	data->_usedMemory = 0;
	data->_currentPos = data->_rootAlloc->data;
}

// TODO: Align Memory into DWORD sized chunks
void* OPallocatorLinearAlloc(OPallocator* alloc, OPuint sizeInBytes) {
	OPallocatorLinear* allocatorLinear = (OPallocatorLinear*)alloc->internalPtr;
	void* data = allocatorLinear->_rootAlloc->data;
	//OPallocAlignAdjustment
	if(allocatorLinear->_usedMemory + sizeInBytes > allocatorLinear->_size) {
	    ASSERT(false, "NO MEMORY LEFT");
		return NULL;
	}
	void* block = allocatorLinear->_currentPos;
	allocatorLinear->_currentPos = (void*)((size_t)block + sizeInBytes);
	allocatorLinear->_usedMemory += sizeInBytes;
	allocatorLinear->_allocCount++;
	return block;
}

