#pragma once

#include "./Data/include/OPgameState.h"

extern OPgameState GS_EXAMPLE_AUDIO;

void ExampleAudioEnter(OPgameState* last);
OPint ExampleAudioUpdate(OPtimer* time);
void ExampleAudioExit(OPgameState* next);