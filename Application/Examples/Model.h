#pragma once

#include "./Data/include/GameStates.h"

extern OPgameState GS_EXAMPLE_MODEL;

void ExampleModelEnter(OPgameState* last);
int ExampleModelUpdate(OPtimer* time);
void ExampleModelExit(OPgameState* next);