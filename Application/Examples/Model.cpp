#include "./Examples/Model.h"
#include "./Human/include/Systems/RenderSystem.h"

#include "./Data/include/ContentManager.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Input/Input.h"

OPgameState GS_EXAMPLE_MODEL = {
	ExampleModelEnter,
	ExampleModelUpdate,
	ExampleModelExit
};

OPmesh* modelMesh;
OPeffect* modelEffect = NULL;
OPcam* modelCamera;
ui32 modelRotation = 1;

void ExampleModelEnter(OPgameState* last) {
	OPcmanLoad("PuzzleBlock.opm");
	OPcmanLoad("SimpleModel.frag");
	OPcmanLoad("SimpleModel.vert");

	modelMesh = (OPmesh*)OPcmanGet("PuzzleBlock.opm");

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 }
	};

	modelEffect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPcmanGet("SimpleModel.vert");
	OPshader* frag = (OPshader*)OPcmanGet("SimpleModel.frag");
	*modelEffect = OPrenderCreateEffectStride(
		*vert,
		*frag,
		attribs,
		2,
		"Model Effect",
		modelMesh->VertexSize
		);

	modelCamera = (OPcam*)OPalloc(sizeof(OPcam));
	*modelCamera = OPcamProj(
		OPvec3One * 2.0,
		OPvec3Create(0, 1, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);
}

int ExampleModelUpdate(OPtimer* time) {
	OPrenderDepth(1);
	OPrenderClear(0, 0, 0);

	OPkeyboardUpdate();
	if (OPkeyboardIsDown(OPKEY_P)) { modelRotation++; }

	OPrenderBindMesh(modelMesh);
	OPrenderBindEffect(modelEffect);

	OPmat4 world, view, proj;
	OPmat4buildRotY(&world, modelRotation / 100.0);

	OPcamGetView((*modelCamera), &view);
	OPcamGetProj((*modelCamera), &proj);

	OPrenderParamMat4v("uWorld", 1, &world);
	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPrenderParamVec3("vLightDirection", 1, &light);

	OPrenderMesh();

	OPrenderPresent();
	return false;
}

void ExampleModelExit(OPgameState* next) {
	OPfree(modelEffect);
	OPfree(modelCamera);
}
