#pragma once
#include "./Data/include/OPgameStates.h"
extern OPgameState State0;
extern OPgameState State1;

void State0Enter(OPgameState* last);
int State0Update(OPtimer* time);
void State0Exit(OPgameState* next);


void State1Enter(OPgameState* last);
int State1Update(OPtimer* time);
void State1Exit(OPgameState* next);