#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Human/include/Rendering/Primitives/OPsphere.h"
#include "./Data/include/OPcman.h"

#ifdef ADDON_openvr
#include "OPopenVR.h"
#endif

// Data for this Game State Example
class OpenVR2Example : public OPgameState {
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

	void Init(OPgameState* last) {

		renderer = OPNEW(OPrendererForward());
		scene.Init(renderer, 100, 1);

		camera.Init();
		scene.camera = &camera.Camera;

		OPsphere::Color = OPvec3(0, 0, 1);
		model = OPsphere::Create(2, ((ui32)OPattributes::POSITION | (ui32)OPattributes::COLOR | (ui32)OPattributes::NORMAL));

		model1Entity = scene.Add(model, true);
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

		OPrenderPresent();
	}

	OPint Exit(OPgameState* next) {

		return 0;
	}
};



// This is for the Example Selector only
OPint GS_EXAMPLE_OPENVR2_AVAILABLE = 1;

// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy
OpenVR2Example _GS_EXAMPLE_OPENVR2;
OPgameState* GS_EXAMPLE_OPENVR2 = &_GS_EXAMPLE_OPENVR2;