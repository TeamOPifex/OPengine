#include "./Examples/Oculus.h"
#include "./Human/include/Systems/RenderSystem.h"

#include "./Data/include/ContentManager.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Input/Input.h"
#include "./Human/include/Input/Oculus.h"

OPgameState GS_EXAMPLE_OCULUS = {
	ExampleOculusEnter,
	ExampleOculusUpdate,
	ExampleOculusExit
};

typedef struct {
	OPmesh* Mesh;
	OPeffect* Effect;
	OPcam* Camera;
	ui32 Rotation;
} OculusExample;

OculusExample* oculusExample;

void ExampleOculusEnter(OPgameState* last) {
	//OPoculusInitialize();

	OPcmanLoad("untitled.opm");
	OPcmanLoad("SimpleModel.frag");
	OPcmanLoad("SimpleModel.vert");

	oculusExample->Mesh = (OPmesh*)OPcmanGet("untitled.opm");

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 }
	};

	oculusExample->Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPcmanGet("SimpleModel.vert");
	OPshader* frag = (OPshader*)OPcmanGet("SimpleModel.frag");
	*oculusExample->Effect = OPrenderCreateEffectStride(
		*vert,
		*frag,
		attribs,
		2,
		"Oculus Effect",
		oculusExample->Mesh->VertexSize
		);

	oculusExample->Camera = (OPcam*)OPalloc(sizeof(OPcam));
	*oculusExample->Camera = OPcamProj(
		OPvec3One * 2.0,
		OPvec3Create(0, 1, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);

}

int ExampleOculusUpdate(OPtimer* time) {


	OPoculusBegin();
	//OPframeBufferBind(&OculusManager->_frameBuffer);
	//OPrenderSetViewport(OculusManager->_ovrTexture[0].OGL.Header.RenderViewport.Pos.x,
	//	OculusManager->_ovrTexture[0].OGL.Header.RenderViewport.Pos.y,
	//	OculusManager->_ovrTexture[0].OGL.Header.RenderViewport.Size.w,
	//	OculusManager->_ovrTexture[0].OGL.Header.RenderViewport.Size.h
	//	);
	OPrenderDepth(1);
	OPrenderClear(0, 0, 0);

	if (OPkeyboardIsDown(OPKEY_P)) { oculusExample->Rotation++; }

	OPrenderBindMesh(oculusExample->Mesh);
	OPrenderBindEffect(oculusExample->Effect);

	OPmat4 world, view, proj;
	OPmat4buildRotY(&world, oculusExample->Rotation / 100.0);

	OPcamGetView((*oculusExample->Camera), &view);
	OPcamGetProj((*oculusExample->Camera), &proj);

	OPrenderParamMat4v("uWorld", 1, &world);
	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPrenderParamVec3("vLightDirection", 1, &light);

	OPrenderMesh();
	OPframeBufferUnbind();

	OPrenderPresent();

	OPoculusEnd();
	return false;
}

void ExampleOculusExit(OPgameState* next) {
	OPfree(oculusExample->Effect);
	OPfree(oculusExample->Camera);

	OPfree(oculusExample);
}