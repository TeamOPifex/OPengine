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
    extern OPuint OPallocationBytes;
    extern OPuint OPallocationBytesRequested;
    extern OPuint OPdeallocationBytes;
	extern void(*OPALLOCATIONTRACKER)(const OPchar*, ui64, const OPchar*, ui64);
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

#ifdef _DEBUG
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
#else
/**
* Platform independent means to dynamically allocate memory.
* @param bytes Number of consecutive bytes to be allocated.
* @return Address of allocated memory.
*/
void* _OPalloc(OPuint bytes);
#define OPalloc(bytes) _OPalloc(bytes)

/**
* Platform independent means to dynamically allocate memory and zero it out.
* @param bytes Number of consecutive bytes to be allocated.
* @return Address of allocated memory.
*/
void* _OPallocZero(OPuint bytes);
#define OPallocZero(bytes) _OPallocZero(bytes)
#endif
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
#ifdef _DEBUG
    void  _OPfree(void* ptr, const OPchar* file, ui32 line, const OPchar* function);
    #define OPfree(bytes) _OPfree(bytes, __FILE__, __LINE__, __FUNCTION__)
#else
    void  _OPfree(void* ptr);
    #define OPfree(bytes) _OPfree(bytes)
#endif

void* OPsysAlloc(OPuint bytes);
void OPsysFree(void* ptr);

// C++ Overloads

#ifdef _DEBUG
    void* operator new(size_t size, const char* file, ui32 line, const char* function);
    void* operator new[](size_t size, const char* file, ui32 line, const char* function);
    void operator delete(void* block, const char* file, ui32 line, const char* function) noexcept;
    void operator delete[](void* block, const char* file, ui32 line, const char* function) noexcept;
    #define OPNEW(x) new(__FILE__, __LINE__, __FUNCTION__) x
#else
    void* operator new(size_t size);
    void* operator new[](size_t size);
    void operator delete(void* block) noexcept;
    void operator delete[](void* block) noexcept;
    #define OPNEW(x) new x
#endif

#define OPALLOC(x,c) (x*)OPalloc(sizeof(x) * c)
