#include "./Pipeline/include/DefaultLoaders.h"
#include "./Pipeline/include/Loaders/OPloaderOPM.h"
#include "./Human/include/Systems/OPaudioSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Human/include/Systems/OPfontSystem.h"
#include "./Human/include/Rendering/Sprite/OPspriteSheet.h"
#include "./Human/include/Utilities/OPimagePNG.h"
#include "./Data/include/OPjson.h"
#include "./Core/include/OPdebug.h"


OPassetLoader OP_DEFAULT_LOADERS[OP_DEFAULT_LOADER_COUNT] = {
	{
		".png",
		"Textures/",
		sizeof(OPtexture),
		(OPint(*)(OPstream*, void**))OPimagePNGLoad,
		(OPint (*)(void*))OPimagePNGUnload,
		(OPint(*)(OPstream*, void**))OPimagePNGReload
	},
	{
		".vert",
#ifdef _DEBUG
	#if defined(OPIFEX_ANDROID) || defined(OPIFEX_IOS)
		"Shaders/OPENGL_ES_2_0/",
	#else
		#ifdef OPIFEX_DIRECTX_11
			"Shaders/DIRECTX_11/",
		#else
			#ifdef OPIFEX_OPENGL_3_3
				"Shaders/OPENGL_3_3/",
			#else
				"Shaders/OPENGL_2_0/",
			#endif
		#endif
	#endif
#else
		"Shaders/",
#endif
		sizeof(OPshader),
#ifdef OPIFEX_DIRECTX_11
		(OPint(*)(OPstream*, void**))OPshaderLoadVertexDX11,
#else
		(OPint(*)(OPstream*, void**))OPshaderLoadVertex,
#endif
		(OPint(*)(void*))OPshaderUnload,
		NULL
	},
	{
		".frag",
#ifdef _DEBUG
	#if defined(OPIFEX_ANDROID) || defined(OPIFEX_IOS)
		"Shaders/OPENGL_ES_2_0/",
	#else
		#ifdef OPIFEX_DIRECTX_11
			"Shaders/DIRECTX_11/",
		#else
			#ifdef OPIFEX_OPENGL_3_3
				"Shaders/OPENGL_3_3/",
			#else
				"Shaders/OPENGL_2_0/",
			#endif
		#endif
	#endif
#else
		"Shaders/",
#endif
		sizeof(OPshader),
#ifdef OPIFEX_DIRECTX_11
		(OPint(*)(OPstream*, void**))OPshaderLoadFragmentDX11,
#else
		(OPint(*)(OPstream*, void**))OPshaderLoadFragment,
#endif
		(OPint(*)(void*))OPshaderUnload,
		NULL
	},
	{
		".opm",
		"Models/",
		sizeof(OPmodel),
		(OPint(*)(OPstream*, void**))OPMloader,
		(OPint(*)(void*))OPMunload,
		(OPint(*)(OPstream*, void**))OPMreload
	},
	{
		".opf",
		"Fonts/",
		sizeof(OPfont),
		(OPint(*)(OPstream*, void**))OPfontLoad,
		(OPint(*)(void*))OPfontUnload,
		NULL
	},
	{
		".opss",
		"Sprites/",
		sizeof(OPspriteSheet),
		(OPint(*)(OPstream*, void**))OPspriteSheetLoad,
		(OPint(*)(void*))OPspriteSheetUnload,
		NULL
	},
	{
		".meta",
		"",
		sizeof(OPjson),
		(OPint(*)(OPstream*, void**))OPjsonCmanLoad,
		(OPint(*)(void*))OPjsonCmanUnload,
		NULL
	},
	{
		".json",
		"",
		sizeof(OPjson),
		(OPint(*)(OPstream*, void**))OPjsonCmanLoad,
		(OPint(*)(void*))OPjsonCmanUnload,
		NULL
	}

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
	TIMED_BLOCK
	for (i32 i = 0; i < OP_DEFAULT_LOADER_COUNT; i++) {
		OPCMAN.AddLoader(&OP_DEFAULT_LOADERS[i]);
	}
}
