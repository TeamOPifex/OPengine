#include "./Pipeline/include/DefaultLoaders.h"
#include "./Pipeline/include/Loaders/OPloaderOPM.h"
#include "./Pipeline/include/Loaders/OPloaderTGA.h"
#include "./Pipeline/include/Loaders/OPloaderBMP.h"
#include "./Pipeline/include/Loaders/OPloaderPNG.h"
#include "./Pipeline/include/Loaders/OPloaderHDR.h"
#include "./Pipeline/include/Loaders/OPloaderOPF.h"
#include "./Pipeline/include/Loaders/OPloaderTexture.h"
#include "./Human/include/Systems/OPaudioSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Human/include/Systems/OPfontSystem.h"
#include "./Human/include/Rendering/Sprite/OPspriteSheet.h"
#include "./Human/include/Utilities/OPimagePNG.h"
//#include "./Data/include/OPjson.h"
#include "./Core/include/OPdebug.h"


void OPloadersAddDefault() {
	TIMED_BLOCK;

    OPCMAN.AddLoader(&OPASSETLOADER_TEXTURE_JPG);
    OPCMAN.AddLoader(&OPASSETLOADER_TEXTURE_JPEG);
	OPCMAN.AddLoader(&OPASSETLOADER_TEXTURE_PNG);
	OPCMAN.AddLoader(&OPASSETLOADER_TEXTURE_TGA);
	OPCMAN.AddLoader(&OPASSETLOADER_TEXTURE_BMP);
	OPCMAN.AddLoader(&OPASSETLOADER_TEXTURE_PSD);
	OPCMAN.AddLoader(&OPASSETLOADER_TEXTURE_GIF);
	OPCMAN.AddLoader(&OPASSETLOADER_HDR);
	OPCMAN.AddLoader(&OPASSETLOADER_VERT);
	OPCMAN.AddLoader(&OPASSETLOADER_FRAG);
	OPCMAN.AddLoader(&OPASSETLOADER_OPM);
	OPCMAN.AddLoader(&OPASSETLOADER_OPF);
	OPCMAN.AddLoader(&OPASSETLOADER_OPSS);
	//OPCMAN.AddLoader(&OPASSETLOADER_META);
	//OPCMAN.AddLoader(&OPASSETLOADER_JSON);
}
