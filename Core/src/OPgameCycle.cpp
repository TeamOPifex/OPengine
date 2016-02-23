#include "../include/OPgameCycle.h"

void (*OPinitialize)();

int(*OPupdate)(struct OPtimer*);

void(*OPrender)(float);

void (*OPdestroy)();
