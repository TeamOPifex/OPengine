#include "./Pipeline/include/DefaultLoaders.h"
#include "./Pipeline/include/Loaders/OPloaderOPM.h"
#include "./Pipeline/include/Loaders/OPloaderTGA.h"
#include "./Pipeline/include/Loaders/OPloaderBMP.h"
#include "./Pipeline/include/Loaders/OPloaderPNG.h"
#include "./Pipeline/include/Loaders/OPloaderOPF.h"
#include "./Human/include/Systems/OPaudioSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Human/include/Systems/OPfontSystem.h"
#include "./Human/include/Rendering/Sprite/OPspriteSheet.h"
#include "./Human/include/Utilities/OPimagePNG.h"
#include "./Data/include/OPjson.h"
#include "./Core/include/OPdebug.h"


void OPloadersAddDefault() {
	TIMED_BLOCK;

	OPCMAN.AddLoader(&OPASSETLOADER_PNG);
	OPCMAN.AddLoader(&OPASSETLOADER_TGA);
	OPCMAN.AddLoader(&OPASSETLOADER_BMP);
	OPCMAN.AddLoader(&OPASSETLOADER_VERT);
	OPCMAN.AddLoader(&OPASSETLOADER_FRAG);
	OPCMAN.AddLoader(&OPASSETLOADER_OPM);
	OPCMAN.AddLoader(&OPASSETLOADER_OPF);
	OPCMAN.AddLoader(&OPASSETLOADER_OPSS);
	OPCMAN.AddLoader(&OPASSETLOADER_META);
	OPCMAN.AddLoader(&OPASSETLOADER_JSON);
}
