#ifndef OPEngine_Core_GameCycle
#define OPEngine_Core_GameCycle
#include "Timer.h"

void (*OPinitialize)();
//----------------------------------------------------------------------------
void (*OPupdate)(OPtimer*);
//----------------------------------------------------------------------------
void (*OPdestroy)();
#endif
