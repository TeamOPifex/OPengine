#pragma once

#include "./Data/include/GameStates.h"

extern OPgameState GS_EXAMPLE_AUDIO;

void ExampleAudioEnter(OPgameState* last);
int ExampleAudioUpdate(OPtimer* time);
void ExampleAudioExit(OPgameState* next);