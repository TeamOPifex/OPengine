#pragma once

#include "./Data/include/OPgameState.h"

extern OPgameState GS_EXAMPLE_SKINNING;

void ExampleSkinningEnter(OPgameState* last);
OPint ExampleSkinningUpdate(OPtimer* time);
void ExampleSkinningExit(OPgameState* next);