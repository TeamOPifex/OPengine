#include "./Examples/Skinning.h"
#include "./Human/include/Systems/RenderSystem.h"

#include "./Data/include/ContentManager.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Rendering/Skinning/Animation.h"

#include "./Human/include/Input/Input.h"

OPgameState GS_EXAMPLE_SKINNING = {
	ExampleSkinningEnter,
	ExampleSkinningUpdate,
	ExampleSkinningExit
};


OPmesh* mesh;
OPeffect* ModelEffect = NULL;
OPcam* skinningCamera;
OPskeleton* skeleton;
OPmat4* bones;

ui32 pos = 0;
f32 timeRot = 0;

void ExampleSkinningEnter(OPgameState* last) {
	OPcmanLoad("mesh.opm");
	OPcmanLoad("SimpleModel.frag");
	OPcmanLoad("SimpleModel.vert");

	mesh = (OPmesh*)OPcmanGet("mesh.opm");


	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 },
		{ "aTangent", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 },
		{ "aBlendIndices", GL_FLOAT, 4 },
		{ "aBlendWeights", GL_FLOAT, 4 }
	};
	ModelEffect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPcmanGet("SimpleModel.vert");
	OPshader* frag = (OPshader*)OPcmanGet("SimpleModel.frag");
	*ModelEffect = OPrenderCreateEffectStride(
		*vert,
		*frag,
		attribs,
		6,
		"Model Effect",
		sizeof(OPMvertexSkin)
		);

	skinningCamera = (OPcam*)OPalloc(sizeof(OPcam));
	*skinningCamera = OPcamProj(
		OPvec3One * 10.0,
		OPvec3Zero,
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);

	bones = (OPmat4*)OPalloc(sizeof(OPmat4)* 15);
	for (i32 i = 0; i < 15; i++)
		OPmat4identity(&bones[i]);
	//OPmat4buildRotX(&bones[2], 1.0);
}

int ExampleSkinningUpdate(OPtimer* time) {
	OPrenderDepth(1);
	OPrenderClear(0, 0, 0);

	OPkeyboardUpdate();
	if (OPkeyboardWasPressed(OPKEY_P)) { pos++; }

	//OPmat4translate(&mesh->Skeleton->localPoses[pos], time->Elapsed / 1000.0f, 0, 0);
	OPmat4rotX(&mesh->Skeleton->localPoses[pos], time->Elapsed / 1000.0f);
	OPskeletonUpdate(mesh->Skeleton);

	OPrenderBindMesh(mesh);
	OPrenderBindEffect(ModelEffect);
	
	OPmat4 world, view, proj;
	OPmat4buildRotX(&world,- OPpi / 2.0);

	OPcamGetView((*skinningCamera), &view);
	OPcamGetProj((*skinningCamera), &proj);

	OPrenderParamMat4v("uWorld", 1, &world);
	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);

	OPrenderParamMat4v("uBones", 15, mesh->Skeleton->globalPoses);

	OPvec3 light = OPvec3Create(0, 10, 0);
	OPrenderParamVec3("uLightPosition", 1, &light);

	OPrenderMesh();

	OPrenderPresent();
	return false;
}

void ExampleSkinningExit(OPgameState* next) {
	OPfree(ModelEffect);
	OPfree(skinningCamera);
}