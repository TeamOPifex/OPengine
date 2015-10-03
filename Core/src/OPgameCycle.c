#include "../include/OPgameCycle.h"

void (*OPinitialize)();

int(*OPupdate)(struct OPtimer*);

void (*OPdestroy)();
