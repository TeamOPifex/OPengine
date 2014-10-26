#pragma once

#include "./Data/include/GameStates.h"

extern OPgameState GS_EXAMPLE_TEXTURED;

void ExampleTexturedEnter(OPgameState* last);
OPint ExampleTexturedUpdate(OPtimer* time);
void ExampleTexturedExit(OPgameState* next);