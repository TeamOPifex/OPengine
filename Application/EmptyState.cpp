#include "./EmptyState.h"

void EmptyStateInit(OPgameState* last) {
}


OPint EmptyStateUpdate(OPtimer* time) {
	OPrenderClear(0,0,0,1);
	OPrenderPresent();
	return false;
}

OPint EmptyStateExit(OPgameState* next) {
	return 0;
}


OPgameState GS_EMPTY = {
	EmptyStateInit,
	EmptyStateUpdate,
	EmptyStateExit
};
