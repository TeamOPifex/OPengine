#pragma once

#include "./Data/include/GameStates.h"

extern OPgameState GS_EXAMPLE_OCULUS;

void ExampleOculusEnter(OPgameState* last);
int ExampleOculusUpdate(OPtimer* time);
void ExampleOculusExit(OPgameState* next);