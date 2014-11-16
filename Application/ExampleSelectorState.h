#pragma once

#include "./Engine.h"

extern OPgameState GS_EXAMPLE_SELECTOR;

void ExampleSelectorEnter(OPgameState* last);
OPint ExampleSelectorUpdate(OPtimer* time);
void ExampleSelectorExit(OPgameState* next);