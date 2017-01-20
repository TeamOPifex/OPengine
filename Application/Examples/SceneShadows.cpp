#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Pipeline/include/OPmaterialPBR.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"

typedef struct {
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
} SceneShadowsExample;

SceneShadowsExample sceneShadowsExample;

void ExampleSceneShadowsEnter(OPgameState* last) {

	sceneShadowsExample.renderer = OPNEW(OPrendererForward());
	//sceneShadowsExample.scene.Init(&sceneShadowsExample.renderer->rendererRoot, 100, 1);
	sceneShadowsExample.camera.Init();
	sceneShadowsExample.scene.camera = &sceneShadowsExample.camera.Camera;

	sceneShadowsExample.model = (OPmodel*)OPCMAN.LoadGet("sponza.opm");

	sceneShadowsExample.model1Entity = sceneShadowsExample.scene.Add(sceneShadowsExample.model, true);
	sceneShadowsExample.DepthTextureEffect.Init("Common/Texture2D.vert", "Common/TextureDepth.frag");
	sceneShadowsExample.DepthTexture = OPtexture2DCreate(&sceneShadowsExample.renderer->depthBuffer.texture, &sceneShadowsExample.DepthTextureEffect);
}

OPint ExampleSceneShadowsUpdate(OPtimer* time) {
	sceneShadowsExample.camera.Update(time);
	return false;
}

void ExampleSceneShadowsRender(OPfloat delta) {
	OPrenderCull(1);
	OPrenderDepth(1);

	OPrenderClear(0.2f, 0.2f, 0.2f);
	sceneShadowsExample.scene.Render(delta);
	OPVISUALDEBUGINFO.Render(delta);
	//OPtexture2DRender(sceneShadowsExample.DepthTexture);
	OPrenderPresent();
}

OPint ExampleSceneShadowsExit(OPgameState* next) {

	return 0;
}

OPint GS_EXAMPLE_SCENE_SHADOWS_AVAILABLE = 1;
OPgameState GS_EXAMPLE_SCENE_SHADOWS = {
	ExampleSceneShadowsEnter,
	ExampleSceneShadowsUpdate,
	ExampleSceneShadowsRender,
	ExampleSceneShadowsExit
};
