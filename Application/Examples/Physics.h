#pragma once

#include "./Data/include/GameStates.h"

extern OPgameState GS_EXAMPLE_PHYSICS;

void ExamplePhysicsEnter(OPgameState* last);
OPint ExamplePhysicsUpdate(OPtimer* time);
void ExamplePhysicsExit(OPgameState* next);