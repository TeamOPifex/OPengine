#include "./Human/include/Systems/AudioSystem.h"
#include "./Human/include/Systems/RenderSystem.h"
#include "./Human/include/Systems/FontSystem.h"
#include "./Human/include/Rendering/Sprite/SpriteSheet.h"
#include "./Human/include/Utilities/LoaderOPM.h"
#include "./Human/include/Utilities/ImagePNG.h"
#include "./Human/include/Utilities/LoaderOPS.h"
#include "./Pipeline/include/DefaultLoaders.h"

OPassetLoader OP_DEFAULT_LOADERS[10] = {
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
		{
			".ops",
			"Scripts/",
			sizeof(OPscript),
			(OPint(*)(const OPchar*, void**))OPscriptLoad,
			(OPint(*)(void*))OPscriptUnload,
			NULL
		},
		{
			".js",
			"Scripts/",
			sizeof(OPscript),
			(OPint(*)(const OPchar*, void**))OPscriptLoad,
			(OPint(*)(void*))OPscriptUnload,
			NULL
		},
};


void OPloadersAddDefault() {
	for (i32 i = 0; i < 10; i++) {
		OPcmanAddLoader(&OP_DEFAULT_LOADERS[i]);
	}
}