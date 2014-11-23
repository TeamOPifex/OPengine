#include "./include/States/Sample.h"

OPgameState GS_SAMPLE = {
	GSsampleEnter,
	GSsampleUpdate,
	GSsampleExit
};

typedef struct {
	ui32 MyData;
} SampleData;

SampleData _sampleData;

void GSsampleEnter(OPgameState* last) {

}

OPint GSsampleUpdate(OPtimer* time) {
	OPrenderClear(0.15, 0.15, 0.15);

	OPrenderPresent();
	return 0;
}

void GSsampleExit(OPgameState* target) {

}
