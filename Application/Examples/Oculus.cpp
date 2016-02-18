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

	OPcmanLoad("untitled.opm");
	OPcmanLoad("SimpleModel.frag");
	OPcmanLoad("SimpleModel.vert");

	oculusExample.Mesh = (OPmesh*)OPcmanGet("untitled.opm");

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 }
	};

	oculusExample.Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPcmanGet("SimpleModel.vert");
	OPshader* frag = (OPshader*)OPcmanGet("SimpleModel.frag");
	*oculusExample.Effect = OPeffectCreate(
		*vert,
		*frag,
		attribs,
		2,
		"Oculus Effect",
		oculusExample.Mesh->VertexSize
		);

	oculusExample.Camera = (OPcam*)OPalloc(sizeof(OPcam));
	*oculusExample.Camera = OPcamPersp(
		OPVEC3_ONE * 2.0,
		OPvec3Create(0, 1, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
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

	if (OPkeyboardIsDown(OPKEY_P)) { oculusExample.Rotation++; }

	OPmeshBind(oculusExample.Mesh);
	OPeffectBind(oculusExample.Effect);

	OPmat4 world;
	world = OPmat4RotY(oculusExample.Rotation / 100.0);
	
	OPeffectParamMat4v("uWorld", 1, &world);
	OPeffectParamMat4v("uProj", 1, &oculusExample.Camera->proj);
	OPeffectParamMat4v("uView", 1, &oculusExample.Camera->view);

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPeffectParamVec3("vLightDirection", &light);

	OPmeshRender();
	OPframeBufferUnbind();

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
