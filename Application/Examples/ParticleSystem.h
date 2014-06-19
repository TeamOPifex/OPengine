#pragma once

#include "./Data/include/GameStates.h"

extern OPgameState GS_EXAMPLE_PARTICLESYSTEM;

void ExampleParticleSystemEnter(OPgameState* last);
int ExampleParticleSystemUpdate(OPtimer* time);
void ExampleParticleSystemExit(OPgameState* next);