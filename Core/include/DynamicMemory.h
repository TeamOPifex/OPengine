#ifndef OPENGINE_DYNMEM
#define OPENGINE_DYNMEM
#include "Target.h" // defines the current build target
#include "Types.h"

#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
#include <stdlib.h> // stdlib is valid for linux, unix, and windows
#elif defined(OPIFEX_ANDROID)
// android specific for malloc
#endif

/* function definitions */
void* OPalloc(OPuint bytes);
void  OPfree(void* ptr);

#endif
