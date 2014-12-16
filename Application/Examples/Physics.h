#pragma once

#include "./Data/include/OPgameState.h"

extern OPgameState GS_EXAMPLE_PHYSICS;

void ExamplePhysicsEnter(OPgameState* last);
OPint ExamplePhysicsUpdate(OPtimer* time);
void ExamplePhysicsExit(OPgameState* next);