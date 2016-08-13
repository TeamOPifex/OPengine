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
	OPuint totalBytes = sizeof(OPallocatorLinear) + sizeInBytes;
	void* data = OPallocZero(totalBytes);

	OPallocatorLinear* alloc = (OPallocatorLinear*)data;
	allocator->data = alloc;

	OPallocatorLinear* linearData = (OPallocatorLinear*)allocator->data;
	_headerStart = linearData;
	_memStart = (void*)((size_t)linearData + sizeof(OPallocatorLinear));
	_size = sizeInBytes;

	allocator->alloc = (void*(*)(void*, OPuint))OPallocatorLinearAlloc;
	allocator->free = (void(*)(void*, void*))OPallocatorLinearFree;
	allocator->clear = (void(*)(void*))OPallocatorLinearReset;
	_rootAlloc = allocator;
	Reset();
}

void OPallocatorLinearFree(OPallocator* alloc, void* data) {

}

void OPallocatorLinearReset(OPallocator* alloc) {
	OPallocatorLinear* data = (OPallocatorLinear*)alloc->data;
	data->_allocCount = 0;
	data->_usedMemory = 0;
	data->_currentPos = data->_memStart;
}

void OPallocatorLinearDestroy(OPallocator* alloc) {
	OPallocatorLinear* data = (OPallocatorLinear*)alloc->data;
	OPfree(data->_headerStart);
}

// TODO: Align Memory into DWORD sized chunks
void* OPallocatorLinearAlloc(OPallocator* alloc, OPuint sizeInBytes) {
	OPallocatorLinear* data = (OPallocatorLinear*)alloc->data;
	//OPallocAlignAdjustment
	if(data->_usedMemory + sizeInBytes > data->_size) {
	    ASSERT(false, "NO MEMORY LEFT");
		return NULL;
	}
	void* block = data->_currentPos;
	data->_currentPos = (void*)((size_t)block + sizeInBytes);
	data->_usedMemory += sizeInBytes;
	data->_allocCount++;
	return block;
}

