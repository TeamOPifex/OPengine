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
				(OPint (*)(void*))OPaudCloseWave
		},
		{
				".ogg",
				"Audio/",
				sizeof(OPaudioSource),
				(OPint (*)(const OPchar*, void**))OPaudOpenOgg,
				(OPint (*)(void*))OPaudCloseOgg
		},
		{
				".png",
				"Textures/",
				sizeof(OPtexture),
				(OPint (*)(const OPchar*, void**))OPimagePNGLoad,
				(OPint (*)(void*))OPimagePNGUnload
		},
		{
				".vert",
#ifdef _DEBUG
#ifdef OPIFEX_ANDROID
				"Shaders/OpenGL_ES_2_0/",
#else
				"Shaders/OpenGL_2_0/",
#endif
#else
				"Shaders/",
#endif
				sizeof(OPshader),
				(OPint (*)(const OPchar*, void**))OPrenderLoadVertexShader,
				(OPint (*)(void*))OPrenderUnloadShader
		},
		{
			".frag",
#ifdef _DEBUG
#ifdef OPIFEX_ANDROID
			"Shaders/OpenGL_ES_2_0/",
#else
			"Shaders/OpenGL_2_0/",
#endif
#else
			"Shaders/",
#endif
				sizeof(OPshader),
				(OPint (*)(const OPchar*, void**))OPrenderLoadFragmentShader,
				(OPint (*)(void*))OPrenderUnloadShader
		},
		{
				".opm",
				"Models/",
				sizeof(OPmesh),
				(OPint (*)(const OPchar*, void**))OPMload,
				(OPint (*)(void*))OPMUnload
		},
		{
				".opf",
				"Fonts/",
				sizeof(OPfont),
				(OPint(*)(const OPchar*, void**))OPfontLoad,
				(OPint(*)(void*))OPfontUnload
		},
		{
				".opss",
				"Sprites/",
				sizeof(OPspriteSheet),
				(OPint(*)(const OPchar*, void**))OPspriteSheetLoad,
				(OPint(*)(void*))OPspriteSheetUnload
		},
		{
			".ops",
			"Scripts/",
			sizeof(OPscript),
			(OPint(*)(const OPchar*, void**))OPscriptLoad,
			(OPint(*)(void*))OPscriptUnload
		},
		{
			".js",
			"Scripts/",
			sizeof(OPscript),
			(OPint(*)(const OPchar*, void**))OPscriptLoad,
			(OPint(*)(void*))OPscriptUnload
		},
};


