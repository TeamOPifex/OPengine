#include "Core.h"

/* OPalloc(..) - Platform independent means to dynamically allocate memory.
 *	Parameters:
 *		uint bytes - Number of consecutive bytes to be allocated.
 *	Returns:
 *		void* - Address of allocated memory.
 */
void* OPalloc(OPuint bytes){
	// allocate memory (for each platform)
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	return malloc(bytes);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	// Windows specific for malloc
	#elif defined(OPIFEX_ANDROID)
	// android specific for malloc
	#endif
}

/* OPfree(..) - Platform independent means do deallocate dynamically
 *		allocated memory. If a null pointer is passed no action
 *		is taken.
 *	Parameters:
 *		void* ptr - Address to the memory segment to deallocate
 *	Returns:
 *		Nothing...
 */
void OPfree(void* ptr){
	// free allocated memory (for each platform)
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	if(ptr) free(ptr);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	// Windows specific for malloc
	#elif defined(OPIFEX_ANDROID)
	// android specific for malloc
	#endif
}
