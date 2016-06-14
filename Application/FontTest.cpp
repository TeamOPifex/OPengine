#include "./FontTest.h"

OPfontManager* fontTestManager;

void FontTestEnter(OPgameState* last) {
    fontTestManager = OPfontManagerSetup("Ubuntu.opf", NULL, 0);
}

OPint FontTestUpdate(OPtimer* time) {

	OPrenderClear(0.1f,0.1f,0.1f,1);
	OPfontRenderBegin(fontTestManager);
	OPfontRender("test", OPvec2(0,0));
	OPfontRenderEnd();

	OPrenderPresent();
	return false;
}
void FontTestRender(OPfloat delta) {

}
OPint FontTestExit(OPgameState* next) {
	return 0;
}


OPgameState GS_FONT_TEST = {
	FontTestEnter,
	FontTestUpdate,
	FontTestRender,
	FontTestExit
};
