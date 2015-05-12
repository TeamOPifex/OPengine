#include "./Pipeline/include/Loaders/OPloaderOPM.h"
#include "./Pipeline/include/DefaultLoaders.h"
#include "./Human/include/Systems/OPaudioSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Human/include/Systems/OPfontSystem.h"
#include "./Human/include/Rendering/Sprite/OPspriteSheet.h"
#include "./Human/include/Utilities/OPimagePNG.h"

#define OP_DEFAULT_LOADER_COUNT 6

OPassetLoader OP_DEFAULT_LOADERS[OP_DEFAULT_LOADER_COUNT] = {
	{
		".png",
		"Textures/",
		sizeof(OPtexture),
		(OPint (*)(const OPchar*, void**))OPimagePNGLoad,
		(OPint (*)(void*))OPimagePNGUnload,
		(OPint(*)(const OPchar*, void**))OPimagePNGReload
	},
	{
		".vert",
#ifdef _DEBUG
#ifdef OPIFEX_ANDROID
		"Shaders/OPENGL_ES_2_0/",
#else
				#ifdef OPIFEX_OPENGL_3_3
				"Shaders/OPENGL_3_3/",
				#else
		"Shaders/OPENGL_2_0/",
				#endif
#endif
#else
		"Shaders/",
#endif
		sizeof(OPshader),
		(OPint (*)(const OPchar*, void**))OPshaderLoadVertex,
		(OPint(*)(void*))OPshaderUnload,
		NULL
	},
	{
		".frag",
#ifdef _DEBUG
#ifdef OPIFEX_ANDROID
		"Shaders/OPENGL_ES_2_0/",
#else
#ifdef OPIFEX_OPENGL_3_3
				"Shaders/OPENGL_3_3/",
#else
		"Shaders/OPENGL_2_0/",
				#endif
#endif
#else
		"Shaders/",
#endif
		sizeof(OPshader),
		(OPint (*)(const OPchar*, void**))OPshaderLoadFragment,
		(OPint(*)(void*))OPshaderUnload,
		NULL
	},
	{
		".opm",
		"Models/",
		sizeof(OPmesh),
		(OPint (*)(const OPchar*, void**))OPMload,
		(OPint(*)(void*))OPMUnload,
		(OPint(*)(const OPchar*, void**))OPMReload
	},
	{
		".opf",
		"Fonts/",
		sizeof(OPfont),
		(OPint(*)(const OPchar*, void**))OPfontLoad,
		(OPint(*)(void*))OPfontUnload,
		NULL
	},
	{
		".opss",
		"Sprites/",
		sizeof(OPspriteSheet),
		(OPint(*)(const OPchar*, void**))OPspriteSheetLoad,
		(OPint(*)(void*))OPspriteSheetUnload,
		NULL
	},
};

//
//{
//	".wav",
//		"Audio/",
//		sizeof(OPaudioSource),
//		(OPint(*)(const OPchar*, void**))OPaudOpenWave,
//		(OPint(*)(void*))OPaudCloseWave,
//		NULL
//},
//{
//	".ogg",
//	"Audio/",
//	sizeof(OPaudioSource),
//	(OPint(*)(const OPchar*, void**))OPaudOpenOgg,
//	(OPint(*)(void*))OPaudCloseOgg,
//	NULL
//},


void OPloadersAddDefault() {
	OPlog("loading");
	for (i32 i = 0; i < OP_DEFAULT_LOADER_COUNT; i++) {
		OPcmanAddLoader(&OP_DEFAULT_LOADERS[i]);
	}
	OPlog("loaded");
}