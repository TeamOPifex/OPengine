#pragma once
#include "./Data/include/OPgameStates.h"
extern OPgameState State0;
extern OPgameState State1;

void State0Enter(OPgameState* last);
void State0Update(OPtimer* time);
void State0Exit(OPgameState* next);


void State1Enter(OPgameState* last);
void State1Update(OPtimer* time);
void State1Exit(OPgameState* next);