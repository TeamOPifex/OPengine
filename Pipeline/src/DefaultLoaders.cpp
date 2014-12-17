#include "./Pipeline/include/Loaders/OPloaderOPM.h"
#include "./Pipeline/include/DefaultLoaders.h"
#include "./Human/include/Systems/OPaudioSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Human/include/Systems/OPfontSystem.h"
#include "./Human/include/Rendering/Sprite/OPspriteSheet.h"
#include "./Human/include/Utilities/ImagePNG.h"

OPassetLoader OP_DEFAULT_LOADERS[8] = {
	{
		".wav",
		"Audio/",
		sizeof(OPaudioSource),
		(OPint (*)(const OPchar*, void**))OPaudOpenWave,
		(OPint(*)(void*))OPaudCloseWave,
		NULL
	},
	{
		".ogg",
		"Audio/",
		sizeof(OPaudioSource),
		(OPint (*)(const OPchar*, void**))OPaudOpenOgg,
		(OPint (*)(void*))OPaudCloseOgg,
		NULL
	},
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
		"Shaders/OPENGL_2_0/",
#endif
#else
		"Shaders/",
#endif
		sizeof(OPshader),
		(OPint (*)(const OPchar*, void**))OPrenderLoadVertexShader,
		(OPint(*)(void*))OPrenderUnloadShader,
		NULL
	},
	{
		".frag",
#ifdef _DEBUG
#ifdef OPIFEX_ANDROID
		"Shaders/OPENGL_ES_2_0/",
#else
		"Shaders/OPENGL_2_0/",
#endif
#else
		"Shaders/",
#endif
		sizeof(OPshader),
		(OPint (*)(const OPchar*, void**))OPrenderLoadFragmentShader,
		(OPint(*)(void*))OPrenderUnloadShader,
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


void OPloadersAddDefault() {
	for (i32 i = 0; i < 8; i++) {
		OPcmanAddLoader(&OP_DEFAULT_LOADERS[i]);
	}
}