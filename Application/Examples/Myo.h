#pragma once

#include "./Data/include/GameStates.h"

extern OPgameState GS_EXAMPLE_MYO;

void ExampleMyoEnter(OPgameState* last);
OPint ExampleMyoUpdate(OPtimer* time);
void ExampleMyoExit(OPgameState* next);