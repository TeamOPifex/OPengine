#pragma once
#ifndef OPEngine_Core
#define OPEngine_Core

#include "Target.h" // defines the current build target
#include "Types.h"
#include "GameCycle.h"
#include "DynamicMemory.h"
#include "Timer.h"
#include "MathHelpers.h"

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
//---- Function prototypes ---------------------------------------------------
void OPstart();
//----------------------------------------------------------------------------
void OPend();
#ifdef __cplusplus
};
#endif
#endif
