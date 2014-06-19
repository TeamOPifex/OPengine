#pragma once

#include "./Data/include/GameStates.h"

extern OPgameState GS_EXAMPLE_SKINNING;

void ExampleSkinningEnter(OPgameState* last);
int ExampleSkinningUpdate(OPtimer* time);
void ExampleSkinningExit(OPgameState* next);