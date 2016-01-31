#include "./EmptyState.h"

void EmptyStateInit(OPgameState* last) {
}


OPint EmptyStateUpdate(OPtimer* time) {
	return false;
}

void EmptyStateRender(OPfloat delta) {
	OPrenderClear(0, 0, 0, 1);
	OPrenderPresent();
}

OPint EmptyStateExit(OPgameState* next) {
	return 0;
}


OPgameState GS_EMPTY = {
	EmptyStateInit,
	EmptyStateUpdate,
	EmptyStateRender,
	EmptyStateExit
};
