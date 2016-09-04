#include "./Human/include/Rendering/OPrendererDeferred.h"
#include "./Human/include/Rendering/Primitives/OPsphere.h"
#include "./Human/include/Rendering/Primitives/OPquad.h"

void OPrendererDeferredInit(OPrenderer* renderer, OPcam** camera, ui32 maxCalls, ui32 maxLights) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;

	renderer->camera = camera;

	deferredRenderer->defaultGBufferEffect = OPNEW(OPeffect("Common/GBuffer.vert", "Common/GBuffer.frag"));
	deferredRenderer->defaultGBufferMaterial = OPNEW(OPmaterial(deferredRenderer->defaultGBufferEffect));
	deferredRenderer->passes[0] = deferredRenderer->defaultGBufferMaterial;
	
	deferredRenderer->defaultLightingEffect = OPNEW(OPeffect("Common/DeferredLighting.vert", "Common/DeferredLighting.frag"));
	deferredRenderer->defaultLightingMaterial = OPNEW(OPmaterial(deferredRenderer->defaultLightingEffect));
	deferredRenderer->passes[1] = deferredRenderer->defaultLightingMaterial;

	deferredRenderer->renderBucket[0].Init(maxCalls, renderer->camera);
	deferredRenderer->renderBucket[1].Init(maxLights, renderer->camera);

	OPtextureDesc gBufferDesc[3];
	// Position
	gBufferDesc[0] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB16F, OPtextureFormat::RGB, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	// Normal
	gBufferDesc[1] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB16F, OPtextureFormat::RGB, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	// Specular
	gBufferDesc[2] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGBA, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::BYTE);
	deferredRenderer->gBuffer.Init(gBufferDesc, 3);


	deferredRenderer->defaultLightingMaterialInstance = OPNEW(OPmaterialInstance(deferredRenderer->defaultLightingMaterial));
	deferredRenderer->defaultLightingMaterialInstance->AddParam("uGbufferPosition", &deferredRenderer->gBuffer.texture[0], 0);
	deferredRenderer->defaultLightingMaterialInstance->AddParam("uGbufferNormal", &deferredRenderer->gBuffer.texture[1], 1);
	deferredRenderer->defaultLightingMaterialInstance->AddParam("uGbufferAlbedoSpec", &deferredRenderer->gBuffer.texture[2], 2);


	deferredRenderer->defaultLightingSpotEffect = OPNEW(OPeffect("Common/DeferredLightingSpot.vert", "Common/DeferredLightingSpot.frag"));
	deferredRenderer->defaultLightingSpotMaterial = OPNEW(OPmaterial(deferredRenderer->defaultLightingSpotEffect));
	deferredRenderer->defaultLightingSpotMaterialInstance = OPNEW(OPmaterialInstance(deferredRenderer->defaultLightingSpotMaterial));
	deferredRenderer->defaultLightingSpotMaterialInstance->AddParam("uGbufferPosition", &deferredRenderer->gBuffer.texture[0], 0);
	deferredRenderer->defaultLightingSpotMaterialInstance->AddParam("uGbufferNormal", &deferredRenderer->gBuffer.texture[1], 1);
	deferredRenderer->defaultLightingSpotMaterialInstance->AddParam("uGbufferAlbedoSpec", &deferredRenderer->gBuffer.texture[2], 2);
	//deferredRenderer->passes[1] = deferredRenderer->defaultLightingSpotMaterial;

	//deferredRenderer->sphereMesh = OPsphere::Create(2, ((ui32)OPattributes::POSITION | (ui32)OPattributes::UV));
	deferredRenderer->quadMesh = OPquadCreate(-1.0f, 1.0f);
	deferredRenderer->sphereMesh = (OPmodel*)OPCMAN.LoadGet("uvsphere20.opm");
}

OPmaterialInstance* OPrendererDeferredCreateMaterialInstance(OPrenderer* renderer, ui32 pass) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	OPmaterialInstance* result = OPNEW(OPmaterialInstance(deferredRenderer->passes[0]));
	return result;
}

void OPrendererDeferredSetMaterial(OPrenderer* renderer, OPmaterial* material, ui32 pass) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	deferredRenderer->passes[pass] = material;
}

void OPrendererDeferredSetMaterialEffect(OPrenderer* renderer, OPeffect* effect, ui32 pass) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	deferredRenderer->passes[pass]->effect = effect;
}

void OPrendererDeferredBegin(OPrenderer* renderer) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	deferredRenderer->gBuffer.Bind();
	OPrenderClear(0, 0, 0);
}

void OPrendererDeferredSubmitModel(OPrenderer* renderer, OPmodel* model, OPmat4* world, OPmaterialInstance* material) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	for (ui32 i = 0; i < model->meshCount; i++) {
		OPrenderCommandDrawIndexed* dc =
			deferredRenderer->renderBucket[0].CreateDrawIndexed()->
			Set(&model->meshes[i], world, material);
		deferredRenderer->renderBucket[0].Submit(dc->key, dc->dispatch, dc);
	}
}

void OPrendererDeferredSubmitMesh(OPrenderer* renderer, OPmesh* mesh, OPmat4* world, OPmaterialInstance* material) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	deferredRenderer->renderBucket[0].Submit(mesh, world, material);
}

void OPrendererDeferredSubmitLight(OPrenderer* renderer, OPlightSpot* light, OPmat4* world) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	deferredRenderer->renderBucket[1].Submit(deferredRenderer->sphereMesh, world, deferredRenderer->defaultLightingSpotMaterialInstance);
}

OPmat4 deferredWorld = OPMAT4_IDENTITY;

void OPrendererDeferredEnd(OPrenderer* renderer) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;

	OPrenderBlend(false);
	deferredRenderer->renderBucket[0].Sort();
	deferredRenderer->renderBucket[0].Flush(false);

	deferredRenderer->gBuffer.Unbind();

	OPrenderClear(0.3);
	OPrenderCull(false);

	// Geometry rendered now do the light pass
	//OPrenderBlend(true);
	deferredRenderer->renderBucket[1].Submit(deferredRenderer->quadMesh, &deferredWorld, deferredRenderer->defaultLightingMaterialInstance);
	deferredRenderer->renderBucket[1].Sort();
	deferredRenderer->renderBucket[1].Flush(false);

}

void OPrendererDeferredPresent(OPrenderer* renderer) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	// Combine GBuffer and Lighting

}

OPrendererDeferred* OPrendererDeferred::Setup() {
	rendererRoot._Init = OPrendererDeferredInit;
	rendererRoot._CreateMaterialInstance = OPrendererDeferredCreateMaterialInstance;
	rendererRoot._SetMaterial = OPrendererDeferredSetMaterial;
	rendererRoot._SetMaterialEffect = OPrendererDeferredSetMaterialEffect;
	rendererRoot._Begin = OPrendererDeferredBegin;
	rendererRoot._SubmitModel = OPrendererDeferredSubmitModel;
	rendererRoot._SubmitMesh = OPrendererDeferredSubmitMesh;
	rendererRoot._SubmitLight = OPrendererDeferredSubmitLight;
	rendererRoot._End = OPrendererDeferredEnd;
	rendererRoot._Present = OPrendererDeferredPresent;

	rendererRoot.internalPtr = this;

	return this;
}

OPrendererDeferred* OPrendererDeferred::Create() {
	OPrendererDeferred* result = OPNEW(OPrendererDeferred());
	return result->Setup();
}

void OPrendererDeferred::Destroy() {
	if (defaultGBufferEffect != NULL) {
		defaultGBufferEffect->Destroy();
		OPfree(defaultGBufferEffect);
	}
	if (defaultGBufferMaterial != NULL) {
		defaultGBufferMaterial->Destroy();
		OPfree(defaultGBufferMaterial);
	}
	if (defaultLightingEffect != NULL) {
		defaultLightingEffect->Destroy();
		OPfree(defaultLightingEffect);
	}
	if (defaultLightingMaterial != NULL) {
		defaultLightingMaterial->Destroy();
		OPfree(defaultLightingMaterial);
	}
	if (defaultLightingSpotEffect != NULL) {
		defaultLightingSpotEffect->Destroy();
		OPfree(defaultLightingSpotEffect);
	}
	if (defaultLightingSpotMaterial != NULL) {
		defaultLightingSpotMaterial->Destroy();
		OPfree(defaultLightingSpotMaterial);
	}

	if (sphereMesh != NULL) {
		sphereMesh->Destroy();
		OPfree(sphereMesh);
	}

	if (quadMesh != NULL) {
		quadMesh->Destroy();
		OPfree(quadMesh);
	}

	gBuffer.Destroy();

	renderBucket[0].Destroy();
	renderBucket[1].Destroy();
}
