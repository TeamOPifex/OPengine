#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Pipeline/include/OPmaterialPBR.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"

class SceneShadowsExample : public OPgameState {
	OPfloat Rotation;
	OPscene scene;
	OPrendererForward* renderer;
	OPmodel* model;
	OPrendererEntity* model1Entity;
	OPcamFreeFlight camera;
	OPmaterialPBR materialPBR;
	OPtextureCube environment;
	OPmaterialPBR* materialInstance;
	OPmaterialPBR* materialInstance2;

	OPmaterial* materialInst1;
	OPmaterial* materialInst2;
	OPeffect DepthTextureEffect;
	OPtexture2DOLD* DepthTexture;

	void Init(OPgameState* last) {

		renderer = OPNEW(OPrendererForward());
		//scene.Init(&renderer->rendererRoot, 100, 1);
		camera.Init();
		scene.camera = &camera.Camera;

		model = (OPmodel*)OPCMAN.LoadGet("sponza.opm");

		model1Entity = scene.Add(model, true);
		DepthTextureEffect.Init("Common/Texture2D.vert", "Common/TextureDepth.frag");
		DepthTexture = OPtexture2DCreate(&renderer->shadowPass.depthBuffer.texture, &DepthTextureEffect);
	}

	OPint Update(OPtimer* time) {
		camera.Update(time);
		return false;
	}

	void Render(OPfloat delta) {
		OPrenderCull(1);
		OPrenderDepth(1);

		OPrenderClear(0.2f, 0.2f, 0.2f);
		scene.Render(delta);
		OPVISUALDEBUGINFO.Render(delta);
		//OPtexture2DRender(DepthTexture);
		OPrenderPresent();
	}

	OPint Exit(OPgameState* next) {

		return 0;
	}
};


OPint GS_EXAMPLE_SCENE_SHADOWS_AVAILABLE = 1;
SceneShadowsExample _GS_EXAMPLE_SCENE_SHADOWS;
OPgameState* GS_EXAMPLE_SCENE_SHADOWS = &_GS_EXAMPLE_SCENE_SHADOWS;