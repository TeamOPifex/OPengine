#include "./Human/include/Rendering/OPrendererForward.h"

void OPrendererForwardInit(OPrenderer* renderer, OPcam** camera, ui32 maxCalls, ui32 maxLights) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;

	renderer->camera = camera;
	//forwardRenderer->passes[0].Init(OPNEW(OPeffect("Common/Texture3D.vert", "Common/Texture.frag")));
	forwardRenderer->passes[0].Init(OPNEW(OPeffect("Common/PBR.vert", "Common/PBR.frag")));
	forwardRenderer->renderBucket[0].Init(maxCalls, renderer->camera);
}

OPmaterialInstance* OPrendererForwardCreateMaterialInstance(OPrenderer* renderer, ui32 pass) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;
	OPmaterialInstance* result = OPNEW(OPmaterialInstance(&forwardRenderer->passes[0]));
	return result;
}

void OPrendererForwardSetMaterialEffect(OPrenderer* renderer, OPeffect* effect, ui32 pass) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;
	forwardRenderer->passes[pass].effect = effect;
}

void OPrendererForwardBegin(OPrenderer* renderer) {

}

void OPrendererForwardSubmit(OPrenderer* renderer, OPmodel* model, OPmaterialInstance* material) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;
	OPrenderCommandDrawIndexed* dc =
		forwardRenderer->renderBucket[0].CreateDrawIndexed()->
		Set(model, material);
	forwardRenderer->renderBucket[0].Submit(dc->key, dc->dispatch, dc);
}

void OPrendererForwardEnd(OPrenderer* renderer) {

}

void OPrendererForwardPresent(OPrenderer* renderer) {
	OPrendererForward* forwardRenderer = (OPrendererForward*)renderer->internalPtr;

	forwardRenderer->renderBucket[0].Sort();
	forwardRenderer->renderBucket[0].Flush(false);

	OPrenderPresent();
}

OPrendererForward* OPrendererForward::Setup() {
	rendererRoot._Init = OPrendererForwardInit;
	rendererRoot._CreateMaterialInstance = OPrendererForwardCreateMaterialInstance;
	rendererRoot._SetMaterialEffect = OPrendererForwardSetMaterialEffect;
	rendererRoot._Begin = OPrendererForwardBegin;
	rendererRoot._Submit = OPrendererForwardSubmit;
	rendererRoot._End = OPrendererForwardEnd;
	rendererRoot._Present = OPrendererForwardPresent;

	rendererRoot.internalPtr = this;

	return this;
}

OPrendererForward* OPrendererForward::Create() {
	OPrendererForward* result = OPNEW(OPrendererForward());
	return result->Setup();
}