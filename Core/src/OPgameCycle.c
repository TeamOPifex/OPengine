#include "../include/OPgameCycle.h"

void (*OPinitialize)();

int(*OPupdate)(struct OPtimer*);
int(*OPupdateStepped)(struct OPtimer*, ui64);
void(*OPrenderStepped)(float);

void (*OPdestroy)();
