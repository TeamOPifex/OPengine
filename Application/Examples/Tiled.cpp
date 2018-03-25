#include "./ExampleSelectorState.h"
#include "./Human/include/Systems/OPrenderSystem.h"

#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Rendering/Enums/OPcullFace.h"

#ifdef ADDON_tmx
    #include "OPtmx.h"
#endif

class TiledExample : public OPgameState {
	OPeffect Effect;
	OPcam Camera;

    #ifdef ADDON_tmx
        OPtmxMap map;
    #endif

	void Init(OPgameState* last) {
        #ifdef ADDON_tmx
            map.Init("Maps/testmap.tmx");
        #else
            OPlogErr("TMX Addon not loaded");
        #endif

		OPCMAN.Load("Common/Tiled.frag");
		OPCMAN.Load("Common/Tiled.vert");

		OPshader* vert = (OPshader*)OPCMAN.Get("Common/Tiled.vert");
		OPshader* frag = (OPshader*)OPCMAN.Get("Common/Tiled.frag");
		Effect.Init(vert, frag);
		Effect.AddUniform("uTexture");
		Effect.AddUniform("uWorld");
		Effect.AddUniform("uProj");
		Effect.AddUniform("uView");

        f32 halfWidth = OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / 4.0f;
        f32 halfHeight = OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height / 4.0f;

		Camera.SetOrtho(OPvec3(0,0,100), OPvec3(0), OPvec3(0,1,0), 0.1, 150.0, 0, halfWidth, 0, halfHeight);
        
        // Camera.SetPerspective(
		// 	OPvec3Create(0, 0, 100),
		// 	OPvec3Create(0, 0, 0),
		// 	OPvec3Create(0, 1, 0),
		// 	0.1f,
		// 	150.0f,
		// 	45.0f,
		// 	OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / (f32)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height
		// );
	}

	OPint Update(OPtimer* time) {
		return false;
	}

	void Render(OPfloat delta) {
		OPrenderClear(0, 0, 0);

		OPmat4 world = OPMAT4_IDENTITY;
		Effect.Bind();
		OPeffectSet("uWorld", &world);
		OPeffectSet("uView", &Camera.view);
		OPeffectSet("uProj", &Camera.proj);
        
		OPrenderCull(false);
		OPrenderDepth(false);

        #ifdef ADDON_tmx
            map.Draw();
        #endif

		OPrenderPresent();
	}

	OPint Exit(OPgameState* next) {
		return 0;
	}
};



OPint GS_EXAMPLE_TILED_AVAILABLE = 1;
TiledExample _GS_EXAMPLE_TILED;
OPgameState* GS_EXAMPLE_TILED = &_GS_EXAMPLE_TILED;
