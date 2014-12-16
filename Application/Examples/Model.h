#pragma once

#include "./Data/include/OPgameState.h"

extern OPgameState GS_EXAMPLE_MODEL;

void ExampleModelEnter(OPgameState* last);
OPint ExampleModelUpdate(OPtimer* time);
void ExampleModelExit(OPgameState* next);