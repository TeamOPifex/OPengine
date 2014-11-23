#ifndef OP_GS_MAINMENU
#define OP_GS_MAINMENU

#include "./Engine.h"

extern OPgameState GS_SAMPLE;

void GSsampleEnter(OPgameState* last);
OPint GSsampleUpdate(OPtimer* time);
void GSsampleExit(OPgameState* target);

#endif
