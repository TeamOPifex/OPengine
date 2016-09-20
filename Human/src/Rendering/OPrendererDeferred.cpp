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
	deferredRenderer->defaultLightingMaterial->depth = false;
	deferredRenderer->defaultLightingMaterial->cull = false;

	deferredRenderer->renderBucket[0].Init(maxCalls, renderer->camera);
	deferredRenderer->renderBucket[1].Init(maxLights, renderer->camera);

	OPtextureDesc gBufferDesc[3];
	// Position
	//gBufferDesc[0] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB16F, OPtextureFormat::RGB, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	gBufferDesc[0] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGBA, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::BYTE);
	// Normal
	//gBufferDesc[1] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB16F, OPtextureFormat::RGB, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	gBufferDesc[1] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGBA, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::BYTE);
	// Specular
	gBufferDesc[2] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGBA, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::BYTE);
	deferredRenderer->gBuffer.Init(gBufferDesc, 3);

	OPtextureDesc lightBufferDec = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGBA, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::BYTE);
	deferredRenderer->lightBuffer.Init(lightBufferDec, &deferredRenderer->gBuffer.depthTexture);

	// Directional light
	deferredRenderer->defaultLightingMaterialInstance = OPNEW(OPmaterialInstance(deferredRenderer->defaultLightingMaterial));

	// Spot light
	deferredRenderer->defaultLightingSpotEffect = OPNEW(OPeffect("Common/DeferredLightingSpot.vert", "Common/DeferredLightingSpot.frag"));
	deferredRenderer->defaultLightingSpotMaterial = OPNEW(OPmaterial(deferredRenderer->defaultLightingSpotEffect));
	deferredRenderer->defaultLightingSpotMaterial->depth = true;
	deferredRenderer->defaultLightingSpotMaterialInstance = OPNEW(OPmaterialInstance(deferredRenderer->defaultLightingSpotMaterial));
	deferredRenderer->defaultLightingSpotMaterialInstance->AddParam("uInvertViewProjection", &deferredRenderer->invertViewProjection);
	deferredRenderer->defaultLightingSpotMaterialInstance->AddParam("uGbufferDepth", &deferredRenderer->gBuffer.depthTexture, 0);

	// Combine Material
	deferredRenderer->defaultCombineEffect = OPNEW(OPeffect("Common/DeferredCombine.vert", "Common/DeferredCombine.frag"));
	deferredRenderer->defaultCombineMaterial = OPNEW(OPmaterial(deferredRenderer->defaultCombineEffect));
	deferredRenderer->defaultCombineMaterial->AddParam("uGbufferPosition", &deferredRenderer->gBuffer.texture[0], 0);
	deferredRenderer->defaultCombineMaterial->AddParam("uGbufferNormal", &deferredRenderer->gBuffer.texture[1], 1);
	deferredRenderer->defaultCombineMaterial->AddParam("uGbufferAlbedoSpec", &deferredRenderer->gBuffer.texture[2], 2);
	deferredRenderer->defaultCombineMaterial->AddParam("uLightBuffer", deferredRenderer->lightBuffer.texture, 3);
	deferredRenderer->defaultCombineMaterial->depth = false;
	deferredRenderer->defaultCombineMaterial->cull = false;


	//deferredRenderer->sphereMesh = OPsphere::Create(2, ((ui32)OPattributes::POSITION | (ui32)OPattributes::UV));
	deferredRenderer->quadMesh = OPquadCreate(1.0f, 1.0f);
	deferredRenderer->sphereMesh = (OPmodel*)OPCMAN.LoadGet("uvsphere20.opm");
}

OPmaterialInstance* OPrendererDeferredCreateMaterialInstance(OPrenderer* renderer, ui32 pass) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	OPmaterialInstance* result = OPNEW(OPmaterialInstance(deferredRenderer->passes[0]));
	return result;
}

OPmaterial* OPrendererDeferredGetMaterial(OPrenderer* renderer, ui32 pass) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	return deferredRenderer->passes[pass];
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

void OPrendererDeferredSubmitModel(OPrenderer* renderer, OPmodel* model, OPmat4* world, OPmaterialInstance** material) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	deferredRenderer->renderBucket[0].Submit(model, world, material);
}

void OPrendererDeferredSubmitModelMaterial(OPrenderer* renderer, OPmodel* model, OPmat4* world, OPmaterialInstance* material) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	deferredRenderer->renderBucket[0].Submit(model, world, material);
}

void OPrendererDeferredSubmitMeshMaterial(OPrenderer* renderer, OPmesh* mesh, OPmat4* world, OPmaterialInstance* material) {
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

	// DRAW SCENE 
	{
		OPrenderBlend(false);
		deferredRenderer->renderBucket[0].Sort();
		deferredRenderer->renderBucket[0].Flush(false);

		deferredRenderer->gBuffer.Unbind();
	}


	// DRAW LIGHTS
	{
		deferredRenderer->invertViewProjection = (*deferredRenderer->rendererRoot.camera)->view * (*deferredRenderer->rendererRoot.camera)->proj;
		OPmat4Inverse(&deferredRenderer->invertViewProjection, deferredRenderer->invertViewProjection);

		deferredRenderer->lightBuffer.Bind();
		f32 ambient = 0.1;
		OPrenderClearColor(ambient, ambient, ambient, 1);

		OPrenderDepthWrite(false);
		OPrenderDepth(true);
		OPrenderBlend(true);
		//OPrenderCullMode(2);


		//OPrenderBlend(true);
		//deferredRenderer->renderBucket[1].Submit(deferredRenderer->quadMesh, &deferredWorld, deferredRenderer->defaultLightingMaterialInstance);
		deferredRenderer->renderBucket[1].Sort();
		deferredRenderer->renderBucket[1].Flush(false);

		deferredRenderer->lightBuffer.Unbind();

		OPrenderCullMode(0);
		OPrenderDepth(true);
		OPrenderDepthWrite(true);
	}

}

void OPrendererDeferredPresent(OPrenderer* renderer) {
	OPrendererDeferred* deferredRenderer = (OPrendererDeferred*)renderer->internalPtr;
	// Combine GBuffer and Lighting

	// COMBINE
	{
		OPrenderClear(0,0,0,1);
		OPrenderCullMode(0);
		OPrenderDepth(false);
		OPrenderDepthWrite(false);

		// Draw fullscreen quad to combine
		deferredRenderer->defaultCombineEffect->Bind();
		deferredRenderer->quadMesh->Bind();
		OPrenderDepth(0);
		OPrenderCull(false);
		OPmat4 world = OPMAT4_IDENTITY;

		OPeffectSet("uGbufferPosition", &deferredRenderer->gBuffer.texture[0], 0);
		OPeffectSet("uGbufferNormal", &deferredRenderer->gBuffer.texture[1], 1);
		OPeffectSet("uGbufferAlbedoSpec", &deferredRenderer->gBuffer.texture[2], 2);
		OPeffectSet("uLightBuffer", deferredRenderer->lightBuffer.texture, 3);
		OPeffectSet("uWorld", 1, &world);

		OPrenderDrawBufferIndexed(0);

		OPrenderCullMode(0);
		OPrenderDepth(true);
		OPrenderDepthWrite(true);
	}
}

OPrendererDeferred* OPrendererDeferred::Setup() {
	rendererRoot._Init = OPrendererDeferredInit;
	rendererRoot._CreateMaterialInstance = OPrendererDeferredCreateMaterialInstance;
	rendererRoot._GetMaterial = OPrendererDeferredGetMaterial;
	rendererRoot._SetMaterial = OPrendererDeferredSetMaterial;
	rendererRoot._SetMaterialEffect = OPrendererDeferredSetMaterialEffect;
	rendererRoot._Begin = OPrendererDeferredBegin;
	rendererRoot._SubmitModel = OPrendererDeferredSubmitModel;
	rendererRoot._SubmitModelMaterial = OPrendererDeferredSubmitModelMaterial;
	rendererRoot._SubmitMeshMaterial = OPrendererDeferredSubmitMeshMaterial;
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
