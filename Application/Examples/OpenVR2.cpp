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
	OPsceneVR scene;
	OPrendererForward* renderer;
	OPmodel* model;
	OPmodel* ground;
	OPrendererEntity* model1Entity;
	OPrendererEntity* model2Entity;
	OPcamFreeFlight camera;

	OPmodel* vrController;
	OPrendererEntity* vrController1Entity;
	OPrendererEntity* vrController2Entity;
#ifdef ADDON_openvr
	OPopenVR openVR;
#endif

	void Init(OPgameState* last) {

		renderer = OPNEW(OPrendererForward());
		scene.Init(renderer, 100, 1);

		camera.Init();
#ifdef ADDON_openvr
		scene.SetCamera(&openVR.leftEye);
		//scene.camera = &camera.Camera;
		scene.SetCamera2(&openVR.rightEye);
#endif

		model = (OPmodel*)OPCMAN.LoadGet("ground_block_2x2x2.fbx.opm");
		ground = OPquadCreateZPlane();
		//model = (OPmodel*)OPCMAN.LoadGet("sponza.opm");
		//OPsphere::Color = OPvec3(0, 0, 1);
		//model = OPsphere::Create(2, ((ui32)OPattributes::POSITION | (ui32)OPattributes::COLOR | (ui32)OPattributes::NORMAL));

		OPrendererEntityDesc desc(false, false, false, false);
		model1Entity = scene.Add(model, desc);
		model1Entity->SetAlbedoMap("Knight.png");
		model2Entity = scene.Add(ground, desc);
		model2Entity->SetAlbedoMap("TetrisBase.png");

		vrController = (OPmodel*)OPCMAN.LoadGet("vrcontrollervive.opm");
		vrController1Entity = scene.Add(vrController, desc);
		vrController2Entity = scene.Add(vrController, desc);
		vrController1Entity->SetAlbedoMap("vrcontrollervive.png");
		vrController2Entity->SetAlbedoMap("vrcontrollervive.png");

		vrController1Entity->world = OPmat4Translate(0, 1, 0);
		vrController2Entity->world = OPmat4Translate(1, 1, 0);

#ifdef ADDON_openvr
		openVR.Init();
		openVR.center.SetPerspective(OPVEC3_ONE, OPVEC3_ZERO);
#endif
	}

	OPint Update(OPtimer* time) {
		camera.Update(time);
		return false;
	}

	void Render(OPfloat delta) {
#ifdef ADDON_openvr
		openVR.Update();
		openVR.UpdateHMDMatrixPose();
#endif

		OPrenderCull(1);
		OPrenderDepth(1);

#ifdef ADDON_openvr
		OPvec3 color = OPvec3(openVR.controller[1].axis2.x, openVR.controller[1].axis1.x, openVR.controller[1].axis1.y);
		OPrenderClear(color);

		model1Entity->world = OPmat4Scl(0.25);
		vrController1Entity->world = openVR.GetTrackedDeviceMatrix(1);
		vrController2Entity->world = openVR.GetTrackedDeviceMatrix(2);

		//openVR.UpdateHMDMatrixPose();

		// Left Eye
		openVR.PrepRenderLeftEye();
		OPrenderClear(color);
		scene.RenderLeft(delta);
		openVR.FinishRenderLeftEye();

		// Right Eye
		openVR.PrepRenderRightEye();
		OPrenderClear(color);
		scene.RenderRight(delta);
		openVR.FinishRenderRightEye();

		// Distort and Composite
		//openVR.RenderDistortion();
		openVR.Composite();


		scene.RenderWith(&openVR.center, delta);// &camera.Camera, delta);

		openVR.WaitForFinish();

		//openVR.UpdateHMDMatrixPose();
#endif
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