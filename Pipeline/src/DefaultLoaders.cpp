#include "./Human/include/Systems/AudioSystem.h"
#include "./Human/include/Systems/RenderSystem.h"
#include "./Human/include/Systems/FontSystem.h"
#include "./Human/include/Utilities/LoaderOPM.h"
#include "./Human/include/Utilities/ImagePNG.h"
#include "./Human/include/Utilities/LoaderOPS.h"
#include "./Pipeline/include/DefaultLoaders.h"

OPassetLoader OP_DEFAULT_LOADERS[8] = {
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
				"Shaders/",
				sizeof(OPshader),
				(OPint (*)(const OPchar*, void**))OPrenderLoadVertexShader,
				(OPint (*)(void*))OPrenderUnloadShader
		},
		{
				".frag",
				"Shaders/",
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
			".ops",
			"Scripts/",
			sizeof(OPscript),
			(OPint(*)(const OPchar*, void**))OPscriptLoad,
			(OPint(*)(void*))OPscriptUnload
		}
};


