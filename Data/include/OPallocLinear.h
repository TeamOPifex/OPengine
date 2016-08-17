#pragma once

struct OPallocatorLinear;
typedef struct OPallocatorLinear OPallocatorLinear;

#include "./Core/include/OPallocator.h"
#include "./Core/include/OPmemory.h"

/**
 * Linear Memory Block
 */
struct OPallocatorLinear {
	OPallocator* _rootAlloc;
	OPuint _allocCount;
	OPuint _usedMemory;
	OPuint _size;
	void* _currentPos;

	OPallocatorLinear() { }
	OPallocatorLinear(OPuint sizeInBytes) { Init(sizeInBytes); }
	OPallocatorLinear(OPallocator* result, OPuint sizeInBytes) { Init(result, sizeInBytes); }

	void Init(OPallocator* result, OPuint sizeInBytes);
	void Destroy();

	inline OPallocator* GetAllocator() { return _rootAlloc; }

	inline void Init(OPuint sizeInBytes) {
		Init(OPNEW(OPallocator()), sizeInBytes);
	}

	inline void* Alloc(OPuint sizeInBytes) {
		return _rootAlloc->alloc(_rootAlloc, sizeInBytes);
	}

	inline void Free(void* data) {
		return _rootAlloc->free(_rootAlloc, data);
	}

	inline void Reset() {
		return _rootAlloc->clear(_rootAlloc);
	}

	inline OPuint Used() {
		return _usedMemory;
	}

	inline static OPallocatorLinear* Create(OPuint sizeInBytes) {
		return OPNEW(OPallocatorLinear(sizeInBytes));
	}
};