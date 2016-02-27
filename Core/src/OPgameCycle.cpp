#include "../include/OPgameCycle.h"

void (*OPinitialize)();

OPint(*OPupdate)(struct OPtimer*);

void(*OPrender)(float);

void (*OPdestroy)();
