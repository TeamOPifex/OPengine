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

typedef struct {
	OPmesh* Mesh;
	OPeffect* Effect;
	OPcam* Camera;
	ui32 Rotation;
} ModelExample;

ModelExample* modelExample;

void ExampleModelEnter(OPgameState* last) {
	OPcmanLoad("PuzzleBlock.opm");
	OPcmanLoad("SimpleModel.frag");
	OPcmanLoad("SimpleModel.vert");

	modelExample = (ModelExample*)OPalloc(sizeof(ModelExample));

	modelExample->Mesh = (OPmesh*)OPcmanGet("PuzzleBlock.opm");

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 }
	};

	modelExample->Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPcmanGet("SimpleModel.vert");
	OPshader* frag = (OPshader*)OPcmanGet("SimpleModel.frag");
	*modelExample->Effect = OPrenderCreateEffectStride(
		*vert,
		*frag,
		attribs,
		2,
		"Model Effect",
		modelExample->Mesh->VertexSize
		);

	modelExample->Camera = (OPcam*)OPalloc(sizeof(OPcam));
	*modelExample->Camera = OPcamProj(
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

	if (OPkeyboardIsDown(OPKEY_P)) { modelExample->Rotation++; }

	OPrenderBindMesh(modelExample->Mesh);
	OPrenderBindEffect(modelExample->Effect);

	OPmat4 world, view, proj;
	OPmat4buildRotY(&world, modelExample->Rotation / 100.0);

	OPcamGetView((*modelExample->Camera), &view);
	OPcamGetProj((*modelExample->Camera), &proj);

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
	OPfree(modelExample->Effect);
	OPfree(modelExample->Camera);

	OPfree(modelExample);
}
