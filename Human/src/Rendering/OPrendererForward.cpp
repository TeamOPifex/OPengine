#include "./Human/include/Rendering/OPrendererForward.h"

void OPrendererForwardInit(OPrenderer* renderer, OPcam** camera, ui32 maxCalls, ui32 maxLights) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;

	renderer->camera = camera;

	forwardRenderer->defaultShadowEffect = OPNEW(OPeffect("Common/DepthRTT.vert", "Common/DepthRTT.frag"));
	forwardRenderer->defaultShadowMaterial = OPNEW(OPmaterial(forwardRenderer->defaultShadowEffect));
	forwardRenderer->defaultShadowMaterial->cullFace = OPcullFace::FRONT;
	forwardRenderer->defaultShadowMaterial->cull = true;
	forwardRenderer->defaultShadowMaterial->depth = true;
	forwardRenderer->defaultShadowMaterialInstance = OPNEW(OPmaterial(forwardRenderer->defaultShadowMaterial));
	const ui32 SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	forwardRenderer->depthBuffer.Init(OPtextureDesc(SHADOW_WIDTH, SHADOW_HEIGHT, OPtextureFormat::DEPTH, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT));

	forwardRenderer->defaultShadowAnimatedEffect = OPNEW(OPeffect("Common/DepthRTTSkinned.vert", "Common/DepthRTT.frag"));
	forwardRenderer->defaultShadowAnimatedMaterial = OPNEW(OPmaterial(forwardRenderer->defaultShadowAnimatedEffect));
	forwardRenderer->defaultShadowAnimatedMaterial->cullFace = OPcullFace::FRONT;
	forwardRenderer->defaultShadowAnimatedMaterial->cull = true;
	forwardRenderer->defaultShadowAnimatedMaterial->depth = true;
	forwardRenderer->defaultShadowAnimatedMaterialInstance = OPNEW(OPmaterial(forwardRenderer->defaultShadowAnimatedMaterial));

	//forwardRenderer->defaultEffect = OPNEW(OPeffect("Common/PBR.vert", "Common/PBR.frag"));
	forwardRenderer->defaultEffect = OPNEW(OPeffect("Common/TexturedShadow.vert", "Common/TexturedShadow.frag"));
	forwardRenderer->defaultMaterial = OPNEW(OPmaterial(forwardRenderer->defaultEffect));

	forwardRenderer->defaultAnimatedEffect = OPNEW(OPeffect("Common/SkinnedShadow.vert", "Common/TexturedShadow.frag"));
	forwardRenderer->defaultAnimatedMaterial = OPNEW(OPmaterial(forwardRenderer->defaultAnimatedEffect));

	forwardRenderer->passes[0] = forwardRenderer->defaultShadowMaterial;
	forwardRenderer->renderBucket[0].Init(maxCalls, renderer->camera);

	// Static Model
	forwardRenderer->passes[1] = forwardRenderer->defaultMaterial;
	forwardRenderer->defaultMaterial->AddParam("uViewShadow", &forwardRenderer->shadowCamera.view);
	forwardRenderer->defaultMaterial->AddParam("uProjShadow", &forwardRenderer->shadowCamera.proj);
	forwardRenderer->defaultMaterial->AddParam("uShadow", &forwardRenderer->depthBuffer.texture, 1);
	forwardRenderer->defaultMaterial->AddParam("uLightPos", &forwardRenderer->shadowCamera.pos);
	forwardRenderer->defaultMaterial->AddParam("uViewPos", &forwardRenderer->shadowCamera.pos);

	// Animated Model
	forwardRenderer->passes[2] = forwardRenderer->defaultAnimatedMaterial;
	forwardRenderer->defaultAnimatedMaterial->AddParam("uViewShadow", &forwardRenderer->shadowCamera.view);
	forwardRenderer->defaultAnimatedMaterial->AddParam("uProjShadow", &forwardRenderer->shadowCamera.proj);
	forwardRenderer->defaultAnimatedMaterial->AddParam("uShadow", &forwardRenderer->depthBuffer.texture, 1);
	forwardRenderer->defaultAnimatedMaterial->AddParam("uLightPos", &forwardRenderer->shadowCamera.pos);
	forwardRenderer->defaultAnimatedMaterial->AddParam("uViewPos", &forwardRenderer->shadowCamera.pos);

	forwardRenderer->renderBucket[1].Init(maxCalls, renderer->camera);		
	//forwardRenderer->shadowCamera.SetPerspective(OPvec3(150, 150, 0), OPVEC3_ZERO, OPVEC3_UP, 1.0f, 500.0f, 45.0f, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height);
	forwardRenderer->shadowCamera.SetOrtho(OPvec3(-120, 155, -1), OPVEC3_ZERO, OPVEC3_UP, 50.0f, 500.0f, -16, 16, -16, 16);
}

OPmaterial* OPrendererForwardCreateMaterial(OPrenderer* renderer, ui32 pass) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;
	OPmaterial* result = OPNEW(OPmaterial(forwardRenderer->passes[0]));
	return result;
}

OPmaterial* OPrendererForwardGetMaterial(OPrenderer* renderer, ui32 pass) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;
	return forwardRenderer->passes[pass];
}

void OPrendererForwardSetMaterial(OPrenderer* renderer, OPmaterial* material, ui32 pass) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;
	forwardRenderer->passes[pass] = material;
}

void OPrendererForwardSetMaterialEffect(OPrenderer* renderer, OPeffect* effect, ui32 pass) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;
	forwardRenderer->passes[pass]->effect = effect;
}

void OPrendererForwardSetMaterials(OPrenderer* renderer, OPrendererEntity* entity) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;
}

void OPrendererForwardSetMaterialsSkeleton(OPrenderer* renderer, OPrendererEntity* entity, OPskeleton* skeleton) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;

}

void OPrendererForwardBegin(OPrenderer* renderer) {

}

void OPrendererForwardSubmitModel(OPrenderer* renderer, OPmodel* model, OPmat4* world, bool shadowed, OPmaterial** material) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;
	forwardRenderer->renderBucket[1].Submit(model, world, material, true);
	if (shadowed) {
		forwardRenderer->renderBucket[0].Submit(model, world, forwardRenderer->defaultShadowMaterialInstance);
	}
}

void OPrendererForwardSubmitModelMaterial(OPrenderer* renderer, OPmodel* model, OPmat4* world, bool shadowed, OPmaterial* material) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;
	forwardRenderer->renderBucket[1].Submit(model, world, material);
	if (shadowed) {
		forwardRenderer->renderBucket[0].Submit(model, world, forwardRenderer->defaultShadowMaterialInstance);
	}
}

void OPrendererForwardSubmitMeshMaterial(OPrenderer* renderer, OPmesh* mesh, OPmat4* world, bool shadowed, OPmaterial* material) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;
	forwardRenderer->renderBucket[1].Submit(mesh, world, material);
	if (shadowed) {
		forwardRenderer->renderBucket[0].Submit(mesh, world, forwardRenderer->defaultShadowMaterialInstance);
	}
}

void OPrendererForwardSubmitRendererEntity(OPrenderer* renderer, OPrendererEntity* rendererEntity) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;
	forwardRenderer->renderBucket[1].Submit(rendererEntity->model, &rendererEntity->world, rendererEntity->material, rendererEntity->desc.materialPerMesh);
	if (rendererEntity->desc.shadowEmitter) {
		forwardRenderer->renderBucket[0].Submit(rendererEntity->model, &rendererEntity->world, rendererEntity->shadowMaterial == NULL ? rendererEntity->shadowMaterial : &forwardRenderer->defaultShadowMaterialInstance, false);
	}
}

void OPrendererForwardEnd(OPrenderer* renderer) {

}

void OPrendererForwardPresent(OPrenderer* renderer) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;

	// Render Pass 0
	{ // Depth Buffer for shadows
		forwardRenderer->depthBuffer.Bind();

		OPrenderClearDepth();
		OPrenderDepth(true);
		OPrenderDepthWrite(true);

		OPcam** start = forwardRenderer->renderBucket[0].camera;
		OPcam* tmp = &forwardRenderer->shadowCamera;;
		forwardRenderer->renderBucket[0].camera = &tmp;

		forwardRenderer->renderBucket[0].Sort();
		forwardRenderer->renderBucket[0].Flush(false);

		forwardRenderer->renderBucket[0].camera = start;

		forwardRenderer->depthBuffer.Unbind();
	}

	// Render Pass 1
	{ // Normal Forward Rendering with shadows
		forwardRenderer->renderBucket[1].Sort();
		forwardRenderer->renderBucket[1].Flush(false);
	}
}

OPrendererForward* OPrendererForward::Setup() {
	rendererRoot._Init = OPrendererForwardInit;
	rendererRoot._CreateMaterial = OPrendererForwardCreateMaterial;
	rendererRoot._GetMaterial = OPrendererForwardGetMaterial;
	rendererRoot._SetMaterial = OPrendererForwardSetMaterial;
	rendererRoot._SetMaterialEffect = OPrendererForwardSetMaterialEffect;
	rendererRoot._SetMaterials = OPrendererForwardSetMaterials;
	rendererRoot._SetMaterialsSkeleton = OPrendererForwardSetMaterialsSkeleton;
	rendererRoot._Begin = OPrendererForwardBegin;
	rendererRoot._SubmitModel = OPrendererForwardSubmitModel;
	rendererRoot._SubmitModelMaterial = OPrendererForwardSubmitModelMaterial;
	rendererRoot._SubmitMeshMaterial = OPrendererForwardSubmitMeshMaterial;
	rendererRoot._SubmitRendererEntity = OPrendererForwardSubmitRendererEntity;
	rendererRoot._End = OPrendererForwardEnd;
	rendererRoot._Present = OPrendererForwardPresent;

	rendererRoot.internalPtr = this;

	return this;
}

OPrendererForward* OPrendererForward::Create() {
	OPrendererForward* result = OPNEW(OPrendererForward());
	return result->Setup();
}

void OPrendererForward::Destroy() {
	if (defaultEffect) {
		defaultEffect->Destroy();
		OPfree(defaultEffect);
	}
	if (defaultMaterial) {
		defaultMaterial->Destroy();
		OPfree(defaultMaterial);
	}
	renderBucket[0].Destroy();
}
