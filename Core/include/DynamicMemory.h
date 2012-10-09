#pragma once
#ifndef OPEngine_Core_DYNMEM
#define OPEngine_Core_DYNMEM

#include "Target.h" // defines the current build target
#include "Types.h"

#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
// android specific for malloc
#include <stdlib.h> // stdlib is valid for linux and unix
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
// Windows specific for malloc
#include <stdlib.h> 
#endif

/* function definitions */
extern void* OPalloc(OPuint bytes);//{return malloc(bytes);}
extern void  OPfree(void* ptr);

#endif
