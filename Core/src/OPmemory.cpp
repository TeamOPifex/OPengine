#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"

#ifndef OPIFEX_OPTION_RELEASE
OPint OPallocations = 0;
OPint OPdeallocations = 0;
#endif

OPchar OPSCRATCHBUFFER[OPSCRATCHBUFFER_SIZE];

void* OPallocatorBasicAlloc(void* alloc, OPuint bytes) {
	#ifndef OPIFEX_OPTION_RELEASE
		OPallocations++;
	#endif

		// allocate memory (for each platform)
	#if defined(OPIFEX_UNIX)
		// android specific for malloc
		return malloc(bytes);
	#elif defined(OPIFEX_WINDOWS)
		// Windows specific for malloc
		return malloc(bytes);
	#endif
}

void OPallocatorBasicFree(void* alloc, void* ptr) {
	#ifndef OPIFEX_OPTION_RELEASE
		OPdeallocations++;
	#endif
	// free allocated memory (for each platform)
	#if defined(OPIFEX_UNIX)
	// android specific for malloc
	if(ptr) free(ptr);
	#elif defined(OPIFEX_WINDOWS)
	// Windows specific for malloc
	if(ptr) free(ptr);
	#endif
}

void OPallocatorBasicReset(void* alloc) { }

OPallocator OPDEFAULT_ALLOCATOR = {
	NULL,
	NULL,
	OPallocatorBasicAlloc,
	OPallocatorBasicFree,
	OPallocatorBasicReset
};

//-----------------------------------------------------------------------------
/* OPalloc(..) - Platform independent means to dynamically allocate memory.
*	Parameters:
*		uint bytes - Number of consecutive bytes to be allocated.
*	Returns:
*		void* - Address of allocated memory.
*/
void* _OPalloc(OPuint bytes, const OPchar* file, ui32 line, const OPchar* function){
	OPlogChannel((ui32)OPlogLevel::MEMORY, "MEMORY", "NEW: %s [%d] (%s)", file, line, function);
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
	OPlogChannel((ui32)OPlogLevel::MEMORY, "MEMORY", "NEW: %s [%d] (%s)", file, line, function);
	result = OPDEFAULT_ALLOCATOR.alloc(&OPDEFAULT_ALLOCATOR, bytes);
 	OPbzero(result, bytes);
 	return result;
}

//-----------------------------------------------------------------------------
/* OPrealloc(..) - Platform independent means to dynamically reallocate memory.
 *	Parameters:
 *		void* ptr - Existing segment of allocated memory
 *		uint  bytes - Number of consecutive bytes to be allocated.
 *	Returns:
 *		void* - Address of allocated memory.
 */
void* OPrealloc(void* ptr, OPuint bytes){
	// allocate memory (for each platform)
	#if defined(OPIFEX_UNIX)
	// android specific for malloc
	return realloc(ptr, bytes);
	#elif defined(OPIFEX_WINDOWS)
	// Windows specific for malloc
	return realloc(ptr, bytes);
	#endif
}
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
	OPlogChannel((ui32)OPlogLevel::MEMORY, "MEMORY", "FREE: %s [%d] (%s)", file, line, function);
	OPDEFAULT_ALLOCATOR.free(&OPDEFAULT_ALLOCATOR, ptr);
}


void* OPsysAlloc(OPuint bytes) {
	return malloc(bytes);
}

void OPsysFree(void* ptr) {
	free(ptr);
}


void* operator new(size_t size)
{
	return OPalloc(size);
}


void* operator new(size_t size, const char* file, ui32 line)
{
	return OPalloc(size, file, line, NULL);
}

void* operator new[](size_t size)
{
	return OPalloc(size);
}

void* operator new[](size_t size, const char* file, ui32 line)
{
	return OPalloc(size);// , file, line);
}


void operator delete(void* block) noexcept
{
	OPfree(block);
}

void operator delete(void* block, const char* file, ui32 line) noexcept
{
	OPfree(block);// , file, line);
}

void operator delete[](void* block) noexcept
{
	OPfree(block);
}

void operator delete[](void* block, const char* file, ui32 line) noexcept
{
	OPfree(block);// , file, line);
}
