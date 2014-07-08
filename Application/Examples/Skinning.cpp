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


OPmesh* skinningMesh;
OPeffect* skinningEffect = NULL;
OPcam* skinningCamera;
OPskeleton* skeleton;

ui32 pos = 0;
f32 timeRot = 0;

void ExampleSkinningEnter(OPgameState* last) {
	OPcmanLoad("untitled.opm");
	OPcmanLoad("Skinning.frag");
	OPcmanLoad("Skinning.vert");

	skinningMesh = (OPmesh*)OPcmanGet("untitled.opm");

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 },
		// { "aTangent", GL_FLOAT, 3 },
		// { "aUV", GL_FLOAT, 2 },
		{ "aBlendIndices", GL_FLOAT, 4 },
		{ "aBlendWeights", GL_FLOAT, 4 }
	};

	skinningEffect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPcmanGet("Skinning.vert");
	OPshader* frag = (OPshader*)OPcmanGet("Skinning.frag");
	*skinningEffect = OPrenderCreateEffectStride(
		*vert,
		*frag,
		attribs,
		4,
		"Model Effect",
		skinningMesh->VertexSize
		);

	skinningCamera = (OPcam*)OPalloc(sizeof(OPcam));
	*skinningCamera = OPcamProj(
		OPvec3One * 2.0,
		OPvec3Create(0, 1, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);
}

int ExampleSkinningUpdate(OPtimer* time) {
	OPrenderDepth(1);
	OPrenderClear(0, 0, 0);

	OPkeyboardUpdate();
	if (OPkeyboardWasPressed(OPKEY_P)) { pos++; }
	if (OPkeyboardWasPressed(OPKEY_O)) { pos--; }

	//OPmat4translate(&mesh->Skeleton->localPoses[pos], time->Elapsed / 1000.0f, 0, 0);
	OPmat4rotX(&skinningMesh->Skeleton->localPoses[pos], OPkeyboardIsDown(OPKEY_W) / 10.0f);
	OPmat4rotX(&skinningMesh->Skeleton->localPoses[pos], OPkeyboardIsDown(OPKEY_S) / -10.0f);
	OPskeletonUpdate(skinningMesh->Skeleton);

	OPrenderBindMesh(skinningMesh);
	OPrenderBindEffect(skinningEffect);
	
	OPmat4 world, view, proj;
	OPmat4identity(&world);
	//OPmat4buildRotX(&world,- OPpi / 2.0);

	OPcamGetView((*skinningCamera), &view);
	OPcamGetProj((*skinningCamera), &proj);

	OPrenderParamMat4v("uWorld", 1, &world);
	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);

	OPrenderParamMat4v("uBones", skinningMesh->Skeleton->hierarchyCount, skinningMesh->Skeleton->skinned);

	OPvec3 light = OPvec3Create(0, 10, 0);
	OPrenderParamVec3("uLightPosition", 1, &light);

	OPrenderMesh();

	OPrenderPresent();
	return false;
}

void ExampleSkinningExit(OPgameState* next) {
	OPfree(skinningEffect);
	OPfree(skinningCamera);
}