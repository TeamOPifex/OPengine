#include "./Pipeline/include/OPrendererFullForward.h"


void OPrendererFullForward::_Init(OPcam** camera, ui32 maxCalls, ui32 maxLights) {
	rendererRoot.camera = camera;
	shadowMaterial.Init(OPNEW(OPeffect("Common/DepthRTT.vert", "Common/DepthRTT.frag")));
	shadowMaterial.cullFace = OPcullFace::FRONT;
	shadowMaterial.cull = shadowMaterial.depth = true;

	shadowSkinnedMaterial.Init(OPNEW(OPeffect("Common/DepthRTTSkinned.vert", "Common/DepthRTT.frag")));
	shadowSkinnedMaterial.cullFace = OPcullFace::FRONT;
	shadowSkinnedMaterial.cull = shadowMaterial.depth = true;

	const ui32 SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	depthBuffer.Init(OPtextureDesc(SHADOW_WIDTH, SHADOW_HEIGHT, OPtextureFormat::DEPTH, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT));

	shadowCamera.SetOrtho(OPvec3(-120, 155, -1), OPVEC3_ZERO, OPVEC3_UP, 50.0f, 500.0f, -16, 16, -16, 16);
	shadowCameraPtr = &shadowCamera;
	renderDepthBucket.Init(maxCalls, &shadowCameraPtr);
	renderBucket.Init(maxCalls, camera);

	texturedMaterial.Init(OPNEW(OPeffect("Common/TexturedShadow.vert", "Common/TexturedShadow.frag")));
	skinnedMaterial.Init(OPNEW(OPeffect("Common/SkinnedShadow.vert", "Common/TexturedShadow.frag")));

	texturedMaterial.rootMaterial.AddParam("uViewShadow", &shadowCamera.view);
	texturedMaterial.rootMaterial.AddParam("uProjShadow", &shadowCamera.proj);
	texturedMaterial.rootMaterial.AddParam("uShadow", &depthBuffer.texture, 1);
	texturedMaterial.rootMaterial.AddParam("uLightPos", &shadowCamera.pos);
	texturedMaterial.rootMaterial.AddParam("uViewPos", &shadowCamera.pos);

	skinnedMaterial.rootMaterial.AddParam("uViewShadow", &shadowCamera.view);
	skinnedMaterial.rootMaterial.AddParam("uProjShadow", &shadowCamera.proj);
	skinnedMaterial.rootMaterial.AddParam("uShadow", &depthBuffer.texture, 1);
	skinnedMaterial.rootMaterial.AddParam("uLightPos", &shadowCamera.pos);
	skinnedMaterial.rootMaterial.AddParam("uViewPos", &shadowCamera.pos);

}

void OPrendererFullForwardInit(OPrenderer* renderer, OPcam** camera, ui32 maxCalls, ui32 maxLights) {
	OPrendererFullForward* forwardRenderer = (OPrendererFullForward*)renderer->internalPtr;
	forwardRenderer->_Init(camera, maxCalls, maxLights);
}

OPmaterialInstance* OPrendererFullForwardCreateMaterialInstance(OPrenderer* renderer, ui32 pass) {
	OPrendererFullForward* forwardRenderer = (OPrendererFullForward*)renderer->internalPtr;
	//OPmaterialInstance* result = OPNEW(OPmaterialInstance(forwardRenderer->passes[0]));
	//return result;
	return NULL;
}

OPmaterial* OPrendererFullForwardGetMaterial(OPrenderer* renderer, ui32 pass) {
	OPrendererFullForward* forwardRenderer = (OPrendererFullForward*)renderer->internalPtr;
	switch (pass) {
	case 0: return &forwardRenderer->shadowMaterial;
	case 1: return &forwardRenderer->shadowSkinnedMaterial;
	case 2: return forwardRenderer->texturedMaterial.Base();
	case 3: return forwardRenderer->skinnedMaterial.Base();
	}
	return NULL;
}

void OPrendererFullForwardSetMaterial(OPrenderer* renderer, OPmaterial* material, ui32 pass) {
	OPrendererFullForward* forwardRenderer = (OPrendererFullForward*)renderer->internalPtr;
	//forwardRenderer->passes[pass] = material;
}

void OPrendererFullForwardSetMaterialEffect(OPrenderer* renderer, OPeffect* effect, ui32 pass) {
	OPrendererFullForward* forwardRenderer = (OPrendererFullForward*)renderer->internalPtr;
	//forwardRenderer->passes[pass]->effect = effect;
}

void OPrendererFullForwardBegin(OPrenderer* renderer) {

}

void OPrendererFullForwardSetMaterials(OPrenderer* renderer, OPrendererEntity* entity) {
	OPrendererFullForward* forwardRenderer = (OPrendererFullForward*)renderer->internalPtr;
	if (!entity->desc.animated) {
		entity->material = forwardRenderer->texturedMaterial.Base()->CreateInstances(entity->model, entity->desc.materialPerMesh);
		entity->shadowMaterial = forwardRenderer->shadowMaterial.CreateInstances(entity->model, entity->desc.materialPerMesh);
	}
	else {
		ui32 count = entity->model->meshCount;
		if (!entity->desc.materialPerMesh) {
			count = 1;
		}
		entity->material = OPALLOC(OPmaterialInstance*, count);

		OPmaterialSkinnedInstance** instances = forwardRenderer->skinnedMaterial.CreateInstances(entity->model, true, entity->desc.materialPerMesh);
		for (ui32 i = 0; i < count; i++) {
			entity->material[i] = instances[i]->Base();
		}
		entity->shadowMaterial = forwardRenderer->shadowSkinnedMaterial.CreateInstances(entity->model, entity->desc.materialPerMesh);
	}
}

void OPrendererFullForwardSetMaterialsSkeleton(OPrenderer* renderer, OPrendererEntity* entity, OPskeleton* skeleton) {
	OPrendererFullForward* forwardRenderer = (OPrendererFullForward*)renderer->internalPtr;

	ui32 count = entity->model->meshCount;
	if (!entity->desc.materialPerMesh) {
		count = 1;
	}
	entity->material = OPALLOC(OPmaterialInstance*, count);

	OPmaterialSkinnedInstance** instances = forwardRenderer->skinnedMaterial.CreateInstances(entity->model, true, entity->desc.materialPerMesh);
	for (ui32 i = 0; i < count; i++) {
		entity->material[i] = instances[i]->Base();
	}

	entity->shadowMaterial = forwardRenderer->shadowSkinnedMaterial.CreateInstances(entity->model, false);

	if (entity->desc.materialPerMesh) {
		for (ui32 i = 0; i < entity->model->meshCount; i++) {
			entity->material[i]->AddBones(skeleton);
		}
	}
	else {
		(*entity->material)->AddBones(skeleton);
	}
	(*entity->shadowMaterial)->AddBones(skeleton);
}

void OPrendererFullForwardSubmitModel(OPrenderer* renderer, OPmodel* model, OPmat4* world, bool shadowed, OPmaterialInstance** material) {
	OPrendererFullForward* forwardRenderer = (OPrendererFullForward*)renderer->internalPtr;
	//forwardRenderer->renderBucket[1].Submit(model, world, material, true);
	//if (shadowed) {
	//	forwardRenderer->renderBucket[0].Submit(model, world, forwardRenderer->defaultShadowMaterialInstance);
	//}
}

void OPrendererFullForwardSubmitModelMaterial(OPrenderer* renderer, OPmodel* model, OPmat4* world, bool shadowed, OPmaterialInstance* material) {
	OPrendererFullForward* forwardRenderer = (OPrendererFullForward*)renderer->internalPtr;
	//forwardRenderer->renderBucket[1].Submit(model, world, material);
	//if (shadowed) {
	//	forwardRenderer->renderBucket[0].Submit(model, world, forwardRenderer->defaultShadowMaterialInstance);
	//}
}

void OPrendererFullForwardSubmitMeshMaterial(OPrenderer* renderer, OPmesh* mesh, OPmat4* world, bool shadowed, OPmaterialInstance* material) {
	OPrendererFullForward* forwardRenderer = (OPrendererFullForward*)renderer->internalPtr;
	//forwardRenderer->renderBucket[1].Submit(mesh, world, material);
	//if (shadowed) {
	//	forwardRenderer->renderBucket[0].Submit(mesh, world, forwardRenderer->defaultShadowMaterialInstance);
	//}
}

void OPrendererFullForwardSubmitRendererEntity(OPrenderer* renderer, OPrendererEntity* rendererEntity) {
	OPrendererFullForward* forwardRenderer = (OPrendererFullForward*)renderer->internalPtr;
	forwardRenderer->renderBucket.Submit(rendererEntity->model, &rendererEntity->world, rendererEntity->material, rendererEntity->desc.materialPerMesh);
	if (rendererEntity->desc.shadowEmitter) {
		forwardRenderer->renderDepthBucket.Submit(rendererEntity->model, &rendererEntity->world, rendererEntity->shadowMaterial, false);
	}
}

void OPrendererFullForwardEnd(OPrenderer* renderer) {

}

void OPrendererFullForwardPresent(OPrenderer* renderer) {
	OPrendererFullForward* forwardRenderer = (OPrendererFullForward*)renderer->internalPtr;

	// Render Pass 0
	{ // Depth Buffer for shadows
		forwardRenderer->depthBuffer.Bind();

		OPrenderClearDepth();
		
		forwardRenderer->renderDepthBucket.Sort();
		forwardRenderer->renderDepthBucket.Flush(false);

		forwardRenderer->depthBuffer.Unbind();
	}

	// Render Pass 1
	{ // Normal Forward Rendering with shadows
		forwardRenderer->renderBucket.Sort();
		forwardRenderer->renderBucket.Flush(false);
	}
}

OPrendererFullForward* OPrendererFullForward::Setup() {
	rendererRoot._Init = OPrendererFullForwardInit;
	rendererRoot._CreateMaterialInstance = OPrendererFullForwardCreateMaterialInstance;
	rendererRoot._GetMaterial = OPrendererFullForwardGetMaterial;
	rendererRoot._SetMaterial = OPrendererFullForwardSetMaterial;
	rendererRoot._SetMaterialEffect = OPrendererFullForwardSetMaterialEffect;
	rendererRoot._SetMaterials = OPrendererFullForwardSetMaterials;
	rendererRoot._SetMaterialsSkeleton = OPrendererFullForwardSetMaterialsSkeleton;
	rendererRoot._Begin = OPrendererFullForwardBegin;
	rendererRoot._SubmitModel = OPrendererFullForwardSubmitModel;
	rendererRoot._SubmitModelMaterial = OPrendererFullForwardSubmitModelMaterial;
	rendererRoot._SubmitMeshMaterial = OPrendererFullForwardSubmitMeshMaterial;
	rendererRoot._SubmitRendererEntity = OPrendererFullForwardSubmitRendererEntity;
	rendererRoot._End = OPrendererFullForwardEnd;
	rendererRoot._Present = OPrendererFullForwardPresent;

	rendererRoot.internalPtr = this;

	return this;
}

OPrendererFullForward* OPrendererFullForward::Create() {
	OPrendererFullForward* result = OPNEW(OPrendererFullForward());
	return result->Setup();
}

void OPrendererFullForward::Destroy() {

}
