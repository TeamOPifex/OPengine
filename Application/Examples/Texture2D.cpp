#include "./ExampleSelectorState.h"
#include "./Human/include/Systems/OPrenderSystem.h"

#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Pipeline/include/OPtexture2D.h"

typedef struct {
	OPtexture2DOLD* Tex;
} Texture2DExample;

Texture2DExample texture2DExample;

void ExampleTexture2DEnter(OPgameState* last) {
    OPtextureOLD* tex = (OPtextureOLD*)OPcmanLoadGet("cubeTex.png");
    texture2DExample.Tex = OPtexture2DCreate(tex, NULL);

    texture2DExample.Tex->Position.y = 250;
}

OPint ExampleTexture2DUpdate(OPtimer* time) {
	return false;
}

void ExampleTexture2DRender(OPfloat delta) {
	OPrenderClear(0, 0, 0);
	OPtexture2DRender(texture2DExample.Tex);
	OPrenderPresent();
}

OPint ExampleTexture2DExit(OPgameState* next) {

	return 0;
}

OPint GS_EXAMPLE_TEXTURE2D_AVAILABLE = 1;
OPgameState GS_EXAMPLE_TEXTURE2D = {
	ExampleTexture2DEnter,
	ExampleTexture2DUpdate,
	ExampleTexture2DRender,
	ExampleTexture2DExit
};
