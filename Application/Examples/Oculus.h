#pragma once

#include "./Data/include/OPgameState.h"

extern OPgameState GS_EXAMPLE_OCULUS;

void ExampleOculusEnter(OPgameState* last);
OPint ExampleOculusUpdate(OPtimer* time);
void ExampleOculusExit(OPgameState* next);