#pragma once

#include "./Data/include/OPgameState.h"

extern OPgameState GS_EXAMPLE_DEFERRED;

void ExampleDeferredEnter(OPgameState* last);
OPint ExampleDeferredUpdate(OPtimer* time);
void ExampleDeferredExit(OPgameState* next);