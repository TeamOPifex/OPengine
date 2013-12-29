#pragma once
#ifndef OPEngine_Core_DYNMEM
#define OPEngine_Core_DYNMEM

#include "Types.h"
#include <string.h>

#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
// android specific for malloc
#include <stdlib.h> // stdlib is valid for linux and unix
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
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

#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
#define OPbzero(dest, size){\
	bzero(dest, size);\
}\

#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
#define OPbzero(dest, size){\
	memset(dest, 0, size);\
}\

#endif

/* function definitions */
// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
/**
 * OPalloc - Platform independent means to dynamically allocate memory.
 * @param bytes Number of consecutive bytes to be allocated.
 * @return Address of allocated memory.
 */
void* OPalloc(OPuint bytes);
//-----------------------------------------------------------------------------
/**
 * OPrealloc - Platform independent means to dynamically reallocate memory.
 * @param ptr Existing segment of allocated memory
 * @param bytes Number of consecutive bytes to be allocated.
 * @return Address of allocated memory.
 */
void* OPrealloc(void* ptr, OPuint bytes);
//-----------------------------------------------------------------------------
/**
 * OPfree - Platform independent means do deallocate dynamically
 *		allocated memory. If a null pointer is passed no action
 *		is taken.
 * @param ptr Address to the memory segment to deallocate
 */
void  OPfree(void* ptr);
#ifdef __cplusplus
}
#endif

#endif
