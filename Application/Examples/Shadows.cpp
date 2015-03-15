#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./OPengine.h"


typedef struct {
	
} ShadowsExample;

ShadowsExample shadowsExample;

void ExampleShadowsEnter(OPgameState* last) {

}

OPint ExampleShadowsUpdate(OPtimer* time) {

	OPrenderClear(0,0,0);

	OPrenderPresent();

	return false;
}

OPint ExampleShadowsExit(OPgameState* next) {
	return 0;
}

OPint GS_EXAMPLE_SHADOWS_AVAILABLE = 1;
// This is the Game State for this ShadowExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_SHADOWS = {
	ExampleShadowsEnter,
	ExampleShadowsUpdate,
	ExampleShadowsExit
};