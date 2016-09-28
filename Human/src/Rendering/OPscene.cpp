#include "./Human/include/Rendering/OPscene.h"

void OPscene::Init(OPrenderer* renderer, ui32 maxEntities, ui32 maxLights) {
	
	index = 0;
	count = maxEntities;
	entities = OPALLOC(OPrendererEntity, count);

	lightIndex = 0;
	lightCount = maxLights;
	lights = OPALLOC(OPsceneLight, lightCount);

	internalCamera.SetPerspective(OPvec3(2), OPvec3(0));
	camera = &internalCamera;

	this->renderer = renderer;
	this->renderer->Init(&camera, maxEntities, 1);
}

OPrendererEntity* OPscene::Add(OPmodel* model, OPmaterialInstance** material, bool materialPerMesh) {
	entities[index].world = OPMAT4_IDENTITY;
	entities[index].model = model;
	entities[index].material = material;
	entities[index].shadowEmitter = true;
	entities[index].shadowReceiver = true;
	entities[index].materialPerMesh = materialPerMesh;
	return &entities[index++];
}

OPrendererEntity* OPscene::Add(OPmodel* model, bool materialPerMesh) {
	entities[index].world = OPMAT4_IDENTITY;
	entities[index].model = model;
	entities[index].material = renderer->GetMaterial(1)->CreateInstances(model, materialPerMesh);
	entities[index].shadowEmitter = true;
	entities[index].shadowReceiver = true;
	return &entities[index++];
}

OPsceneLight* OPscene::Add(OPlightSpot light) {
	lights[lightIndex].light = light;
	return &lights[lightIndex++];
}

OPint OPscene::Update(OPtimer* timer) {
	return 0;
}

void OPscene::Render(OPfloat delta) {

	renderer->Begin();

	for (ui32 i = 0; i < index; i++) {
		renderer->Submit(&entities[i]);
	}

	for (ui32 i = 0; i < lightIndex; i++) {
		renderer->Submit(&lights[i].light);
	}

	renderer->End();

	renderer->Present();

}

void OPscene::Destroy() {
	OPfree(entities);
}