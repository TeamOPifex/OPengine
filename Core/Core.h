#ifndef OPEngine_Core
#define OPEngine_Core

#include <stdlib.h>
#include "Types.h"
#include "Timer.h"
#include "GameCycle.h"

void* OPalloc(ui32 bytes);
void  OPfree(void* ptr);

#endif
