#ifndef OPEngine_Core_DYNMEM
#define OPEngine_Core_DYNMEM

#include "OPtypes.h"
#include "OPallocator.h"
#include <string.h>

#define OPSCRATCHBUFFER_SIZE 4096
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

/* function definitions */
// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

/**
* Platform independent means to dynamically allocate memory.
* @param bytes Number of consecutive bytes to be allocated.
* @return Address of allocated memory.
*/
void* OPalloc(OPuint bytes);

/**
* Platform independent means to dynamically allocate memory and zero it out.
* @param bytes Number of consecutive bytes to be allocated.
* @return Address of allocated memory.
*/
void* OPallocZero(OPuint bytes);

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
void  OPfree(void* ptr);

void* OPsysAlloc(OPuint bytes);
void OPsysFree(void* ptr);

#ifdef __cplusplus
}
#endif

#endif
