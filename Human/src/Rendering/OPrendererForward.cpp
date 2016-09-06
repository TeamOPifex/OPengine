#include "./Human/include/Rendering/OPrendererForward.h"

void OPrendererForwardInit(OPrenderer* renderer, OPcam** camera, ui32 maxCalls, ui32 maxLights) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;

	renderer->camera = camera;

	forwardRenderer->defaultEffect = OPNEW(OPeffect("Common/PBR.vert", "Common/PBR.frag"));
	forwardRenderer->defaultMaterial = OPNEW(OPmaterial(forwardRenderer->defaultEffect));
	forwardRenderer->passes[0] = forwardRenderer->defaultMaterial;
	forwardRenderer->renderBucket[0].Init(maxCalls, renderer->camera);
}

OPmaterialInstance* OPrendererForwardCreateMaterialInstance(OPrenderer* renderer, ui32 pass) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;
	OPmaterialInstance* result = OPNEW(OPmaterialInstance(forwardRenderer->passes[0]));
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

void OPrendererForwardBegin(OPrenderer* renderer) {

}

void OPrendererForwardSubmitModel(OPrenderer* renderer, OPmodel* model, OPmat4* world, OPmaterialInstance** material) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;
	forwardRenderer->renderBucket[0].Submit(model, world, material);
}

void OPrendererForwardSubmitModelMaterial(OPrenderer* renderer, OPmodel* model, OPmat4* world, OPmaterialInstance* material) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;
	forwardRenderer->renderBucket[0].Submit(model, world, material);
}

void OPrendererForwardSubmitMeshMaterial(OPrenderer* renderer, OPmesh* mesh, OPmat4* world, OPmaterialInstance* material) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;
	forwardRenderer->renderBucket[0].Submit(mesh, world, material);
}

void OPrendererForwardEnd(OPrenderer* renderer) {

}

void OPrendererForwardPresent(OPrenderer* renderer) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;
	forwardRenderer->renderBucket[0].Sort();
	forwardRenderer->renderBucket[0].Flush(false);
}

OPrendererForward* OPrendererForward::Setup() {
	rendererRoot._Init = OPrendererForwardInit;
	rendererRoot._CreateMaterialInstance = OPrendererForwardCreateMaterialInstance;
	rendererRoot._GetMaterial = OPrendererForwardGetMaterial;
	rendererRoot._SetMaterial = OPrendererForwardSetMaterial;
	rendererRoot._SetMaterialEffect = OPrendererForwardSetMaterialEffect;
	rendererRoot._Begin = OPrendererForwardBegin;
	rendererRoot._SubmitModel = OPrendererForwardSubmitModel;
	rendererRoot._SubmitModelMaterial = OPrendererForwardSubmitModelMaterial;
	rendererRoot._SubmitMeshMaterial = OPrendererForwardSubmitMeshMaterial;
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
