#pragma once

#include "./Data/include/GameStates.h"

extern OPgameState GS_EXAMPLE_DEFERRED;

void ExampleDeferredEnter(OPgameState* last);
int ExampleDeferredUpdate(OPtimer* time);
void ExampleDeferredExit(OPgameState* next);