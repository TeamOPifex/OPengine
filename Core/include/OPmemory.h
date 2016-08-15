#pragma once

#include "OPtypes.h"
#include "OPallocator.h"
#include <string.h>

#define OPSCRATCHBUFFER_SIZE 4096

// Global buffer to use for temporary data
// DO NOT USE WITH THREADING
extern OPchar OPSCRATCHBUFFER[OPSCRATCHBUFFER_SIZE];

#ifndef OPIFEX_OPTION_RELEASE
	extern OPint OPallocations;
	extern OPint OPdeallocations;
#endif

extern OPallocator OPDEFAULT_ALLOCATOR;

#if defined(OPIFEX_UNIX)
// android specific for malloc
#include <stdlib.h> // stdlib is valid for linux and unix
#elif defined(OPIFEX_WINDOWS)
// Windows specific for malloc
#include <stdlib.h>
#endif

// Define preprocessor macros
#define OPmemcpy(dest, src, size){\
	memcpy(dest, src, size);\
}\

#define OPmemmove(dest, src, size){\
	memmove(dest, src, size); \
}\

#define OPmemcmp(dest, src, size) memcmp(dest, src, size)

// TODO: (garrett) bzero could be optimized out, I think this should use a memset type operation instead.
#if defined(OPIFEX_WINDOWS)
#define OPbzero(dest, size) memset(dest, 0, size);
#else
#define OPbzero(dest, size) bzero(dest, size);
#endif


/**
* Platform independent means to dynamically allocate memory.
* @param bytes Number of consecutive bytes to be allocated.
* @return Address of allocated memory.
*/
void* _OPalloc(OPuint bytes, const OPchar* file, ui32 line, const OPchar* function);
#define OPalloc(bytes) _OPalloc(bytes, __FILE__, __LINE__, __FUNCTION__)

/**
* Platform independent means to dynamically allocate memory and zero it out.
* @param bytes Number of consecutive bytes to be allocated.
* @return Address of allocated memory.
*/
void* _OPallocZero(OPuint bytes, const OPchar* file, ui32 line, const OPchar* function);
#define OPallocZero(bytes) _OPallocZero(bytes, __FILE__, __LINE__, __FUNCTION__)

//-----------------------------------------------------------------------------
/**
 * Platform independent means to dynamically reallocate memory.
 * @param ptr Existing segment of allocated memory
 * @param bytes Number of consecutive bytes to be allocated.
 * @return Address of allocated memory.
 */
void* OPrealloc(void* ptr, OPuint bytes);

//-----------------------------------------------------------------------------
/**
 * Platform independent means do deallocate dynamically
 *		allocated memory. If a null pointer is passed no action
 *		is taken.
 * @param ptr Address to the memory segment to deallocate
 */
void  _OPfree(void* ptr, const OPchar* file, ui32 line, const OPchar* function);
#define OPfree(bytes) _OPfree(bytes, __FILE__, __LINE__, __FUNCTION__)

void* OPsysAlloc(OPuint bytes);
void OPsysFree(void* ptr);

// C++ Overloads

void* operator new(size_t size);
void* operator new(size_t size, const char* file, ui32 line);
void* operator new[](size_t size);
void* operator new[](size_t size, const char* file, ui32 line);
void operator delete(void* block) noexcept;
void operator delete(void* block, const char* file, ui32 line) noexcept;
void operator delete[](void* block) noexcept;
void operator delete[](void* block, const char* file, ui32 line) noexcept;

#define OPNEW(x) new(__FILE__, __LINE__) x
#define OPALLOC(x,c) (x*)OPalloc(sizeof(x) * c)
