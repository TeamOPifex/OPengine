#ifndef OPEngine_Core
#define OPEngine_Core

#include "Target.h" // defines the current build target

#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
#include <stdlib.h> // stdlib is valid for linux and unix
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
// Windows specific for malloc
#elif defined(OPIFEX_ANDROID)
// android specific for malloc
#endif

#include "Types.h"
#include "Timer.h"
#include "GameCycle.h"

void* OPalloc(uint bytes);
void  OPfree(void* ptr);

#endif
