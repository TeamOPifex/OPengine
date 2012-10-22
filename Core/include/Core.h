#pragma once
#ifndef OPEngine_Core
#define OPEngine_Core

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
#include "Target.h" // defines the current build target
#include "Types.h"
#include "DynamicMemory.h"
#include "Timer.h"
#include "MathHelpers.h"
#include "GameCycle.h"

//---- Function prototypes ---------------------------------------------------
void OPstart();
//----------------------------------------------------------------------------
void OPend();
#ifdef __cplusplus
}
#endif
#endif
