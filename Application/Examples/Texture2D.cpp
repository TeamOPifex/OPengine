#include "./ExampleSelectorState.h"
#include "./Human/include/Systems/OPrenderSystem.h"

#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Pipeline/include/OPtexture2D.h"

class Texture2DExample : public OPgameState {
	OPtexture2DOLD* Tex;


	void Init(OPgameState* last) {
		OPtexture* tex = (OPtexture*)OPCMAN.LoadGet("TetrisBlue.psd");
		Tex = OPtexture2DCreate(tex, NULL);

		Tex->Position.y = 250;
	}

	OPint Update(OPtimer* time) {
		return false;
	}

	void Render(OPfloat delta) {
		OPrenderClear(0, 0, 0);
		OPtexture2DRender(Tex);
		OPrenderPresent();
	}

	OPint Exit(OPgameState* next) {

		return 0;
	}
} ;


OPint GS_EXAMPLE_TEXTURE2D_AVAILABLE = 1;
Texture2DExample _GS_EXAMPLE_TEXTURE2D;
OPgameState* GS_EXAMPLE_TEXTURE2D = &_GS_EXAMPLE_TEXTURE2D;