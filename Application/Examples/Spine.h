#pragma once

#include "./Data/include/OPgameState.h"

extern OPgameState GS_EXAMPLE_SPINE;

void ExampleSpineEnter(OPgameState* last);
OPint ExampleSpineUpdate(OPtimer* time);
void ExampleSpineExit(OPgameState* next);