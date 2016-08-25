#include "./ExampleSelectorState.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Human/include/Systems/OPinputSystem.h"

#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Input/OPoculus.h"

typedef struct {
	OPmesh* Mesh;
	OPeffect* Effect;
	OPcam* Camera;
	ui32 Rotation;
} OculusExample;

OculusExample oculusExample;

void ExampleOculusEnter(OPgameState* last) {

	OPoculusInitialize();

	OPCMAN.Load("untitled.opm");
	OPCMAN.Load("SimpleModel.frag");
	OPCMAN.Load("SimpleModel.vert");

	oculusExample.Mesh = (OPmesh*)OPCMAN.Get("untitled.opm");

	OPshaderAttribute attribs[] = {
		{ "aPosition", OPshaderElementType::FLOAT, 3 },
		{ "aNormal", OPshaderElementType::FLOAT, 3 }
	};

	oculusExample.Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPCMAN.Get("SimpleModel.vert");
	OPshader* frag = (OPshader*)OPCMAN.Get("SimpleModel.frag");
	oculusExample.Effect->Init(vert, frag);

	oculusExample.Camera = (OPcam*)OPalloc(sizeof(OPcam));
	oculusExample.Camera->SetPerspective(
		OPVEC3_ONE * 2.0,
		OPvec3Create(0, 1, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / (f32)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height
		);

}

OPint ExampleOculusUpdate(OPtimer* time) {


	OPoculusBegin();
	//OPframeBufferBind(&OculusManager->_frameBuffer);
	//OPrenderSetViewport(OculusManager->_ovrTexture[0].OGL.Header.RenderViewport.Pos.x,
	//	OculusManager->_ovrTexture[0].OGL.Header.RenderViewport.Pos.y,
	//	OculusManager->_ovrTexture[0].OGL.Header.RenderViewport.Size.w,
	//	OculusManager->_ovrTexture[0].OGL.Header.RenderViewport.Size.h
	//	);
	OPrenderDepth(1);
	OPrenderClear(0, 0, 0);

	if (OPKEYBOARD.IsDown(OPkeyboardKey::P)) { oculusExample.Rotation++; }

	oculusExample.Mesh->Bind();
	oculusExample.Effect->Bind();

	OPmat4 world;
	world = OPmat4RotY(oculusExample.Rotation / 100.0f);

	OPeffectSet("uWorld", 1, &world);
	OPeffectSet("uProj", 1, &oculusExample.Camera->proj);
	OPeffectSet("uView", 1, &oculusExample.Camera->view);

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPeffectSet("vLightDirection", &light);

	OPrenderDrawBufferIndexed(0);

	//OPframeBufferUnbind();

	OPrenderPresent();

	OPoculusEnd();
	return false;
}

OPint ExampleOculusExit(OPgameState* next) {
	OPfree(oculusExample.Effect);
	OPfree(oculusExample.Camera);

	return 0;
}

void ExampleOculusRender(OPfloat delta) {

}

#ifndef OPIFEX_OPTION_OCULUS
OPint GS_EXAMPLE_OCULUS_AVAILABLE = 0;
#else
OPint GS_EXAMPLE_OCULUS_AVAILABLE = 1;
#endif

OPgameState GS_EXAMPLE_OCULUS = {
	ExampleOculusEnter,
	ExampleOculusUpdate,
	ExampleOculusRender,
	ExampleOculusExit
};
