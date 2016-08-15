#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"

#ifndef OPIFEX_OPTION_RELEASE
OPint OPallocations = 0;
OPint OPdeallocations = 0;
OPuint OPallocationBytes = 0;
OPuint OPallocationBytesRequested = 0;
OPuint OPdeallocationBytes = 0;
#endif

#ifdef OPIFEX_OSX
#include <malloc/malloc.h>
#define OPMALLOC_SIZE(p) malloc_size(p)
#else
#define OPMALLOC_SIZE(p) 0
#endif

OPchar OPSCRATCHBUFFER[OPSCRATCHBUFFER_SIZE];

void* OPallocatorBasicAlloc(void* alloc, OPuint bytes) {
    void* result = malloc(bytes);

	#ifndef OPIFEX_OPTION_RELEASE
		OPallocations++;
        OPallocationBytesRequested += bytes;
        OPallocationBytes += OPMALLOC_SIZE(result);
	#endif

	return result;
}

void OPallocatorBasicFree(void* alloc, void* ptr) {
	#ifndef OPIFEX_OPTION_RELEASE
		OPdeallocations++;
        OPdeallocationBytes += OPMALLOC_SIZE(ptr);
	#endif

	if(ptr) free(ptr);
}

void OPallocatorBasicReset(void* alloc) { }

OPallocator OPDEFAULT_ALLOCATOR = {
	NULL,
	NULL,
	OPallocatorBasicAlloc,
	OPallocatorBasicFree,
	OPallocatorBasicReset
};

#ifdef _DEBUG
//-----------------------------------------------------------------------------
/* OPalloc(..) - Platform independent means to dynamically allocate memory.
*	Parameters:
*		uint bytes - Number of consecutive bytes to be allocated.
*	Returns:
*		void* - Address of allocated memory.
*/
void* _OPalloc(OPuint bytes, const OPchar* file, ui32 line, const OPchar* function){
	OPlogChannel((ui32)OPlogLevel::MEMORY, "MEMORY", "NEW: %s:%d (%s) %d bytes", file, line, function, bytes);
	return OPDEFAULT_ALLOCATOR.alloc(&OPDEFAULT_ALLOCATOR, bytes);
}

//-----------------------------------------------------------------------------
/* OPallocZero(..) - Platform independent means to dynamically allocate memory and zero it out.
*	Parameters:
*		uint bytes - Number of consecutive bytes to be allocated.
*	Returns:
*		void* - Address of allocated memory.
*/
void* _OPallocZero(OPuint bytes, const OPchar* file, ui32 line, const OPchar* function){
	void* result;
	OPlogChannel((ui32)OPlogLevel::MEMORY, "MEMORY", "NEW: %s:%d (%s) %d bytes", file, line, function, bytes);
	result = OPDEFAULT_ALLOCATOR.alloc(&OPDEFAULT_ALLOCATOR, bytes);
 	OPbzero(result, bytes);
 	return result;
}
#else
//-----------------------------------------------------------------------------
/* OPalloc(..) - Platform independent means to dynamically allocate memory.
*	Parameters:
*		uint bytes - Number of consecutive bytes to be allocated.
*	Returns:
*		void* - Address of allocated memory.
*/
void* _OPalloc(OPuint bytes){
	return OPDEFAULT_ALLOCATOR.alloc(&OPDEFAULT_ALLOCATOR, bytes);
}

//-----------------------------------------------------------------------------
/* OPallocZero(..) - Platform independent means to dynamically allocate memory and zero it out.
*	Parameters:
*		uint bytes - Number of consecutive bytes to be allocated.
*	Returns:
*		void* - Address of allocated memory.
*/
void* _OPallocZero(OPuint bytes){
	void* result;
	result = OPDEFAULT_ALLOCATOR.alloc(&OPDEFAULT_ALLOCATOR, bytes);
 	OPbzero(result, bytes);
 	return result;
}
#endif

//-----------------------------------------------------------------------------
/* OPrealloc(..) - Platform independent means to dynamically reallocate memory.
 *	Parameters:
 *		void* ptr - Existing segment of allocated memory
 *		uint  bytes - Number of consecutive bytes to be allocated.
 *	Returns:
 *		void* - Address of allocated memory.
 */
void* OPrealloc(void* ptr, OPuint bytes) {

	#ifndef OPIFEX_OPTION_RELEASE
        OPuint initialSize = OPMALLOC_SIZE(ptr);
        // Kind of a rough approximation right now
        OPallocationBytesRequested += bytes - initialSize;
	#endif
    void* result = realloc(ptr, bytes);

	#ifndef OPIFEX_OPTION_RELEASE
        OPallocationBytes += OPMALLOC_SIZE(result) - initialSize;
    #endif
    return result;
}

#ifdef _DEBUG
//-----------------------------------------------------------------------------
/* OPfree(..) - Platform independent means do deallocate dynamically
 *		allocated memory. If a null pointer is passed no action
 *		is taken.
 *	Parameters:
 *		void* ptr - Address to the memory segment to deallocate
 *	Returns:
 *		Nothing...
 */
void _OPfree(void* ptr, const OPchar* file, ui32 line, const OPchar* function){
	OPlogChannel((ui32)OPlogLevel::MEMORY, "MEMORY", "FREE: %s:%d (%s) %d bytes", file, line, function, OPMALLOC_SIZE(ptr));
	OPDEFAULT_ALLOCATOR.free(&OPDEFAULT_ALLOCATOR, ptr);
}
#else
void _OPfree(void* ptr){
	OPDEFAULT_ALLOCATOR.free(&OPDEFAULT_ALLOCATOR, ptr);
}
#endif

void* OPsysAlloc(OPuint bytes) {
	return malloc(bytes);
}

void OPsysFree(void* ptr) {
	free(ptr);
}

#ifdef _DEBUG

void* operator new(size_t size, const char* file, ui32 line)
{
	return _OPalloc(size, file, line, NULL);
}

void* operator new[](size_t size, const char* file, ui32 line)
{
	return _OPalloc(size, file, line, NULL);
}


void operator delete(void* block, const char* file, ui32 line) noexcept
{
	_OPfree(block, file, line, NULL);
}

void operator delete[](void* block, const char* file, ui32 line) noexcept
{
	_OPfree(block, file, line, NULL);
}

#else

void* operator new(size_t size)
{
	return _OPalloc(size, NULL, 0, NULL);
}

void* operator new[](size_t size)
{
	return _OPalloc(size, NULL, 0, NULL);
}

void operator delete(void* block) noexcept
{
	OPfree(block);
}

void operator delete[](void* block) noexcept
{
	OPfree(block);
}

#endif
