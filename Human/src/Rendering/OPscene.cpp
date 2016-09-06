#include "./Human/include/Rendering/OPscene.h"

void OPscene::Init(OPrenderer* renderer, ui32 maxEntities, ui32 maxLights) {
	
	index = 0;
	count = maxEntities;
	entities = OPALLOC(OPsceneEntity, count);

	lightIndex = 0;
	lightCount = maxLights;
	lights = OPALLOC(OPsceneLight, lightCount);

	internalCamera.SetPerspective(OPvec3(2), OPvec3(0));
	camera = &internalCamera;

	this->renderer = renderer;
	this->renderer->Init(&camera, maxEntities, 1);
}

OPsceneEntity* OPscene::Add(OPmodel* model, OPmaterialInstance** material) {
	entities[index].model = model;
	entities[index].material = material;
	return &entities[index++];
}

OPsceneEntity* OPscene::Add(OPmodel* model) {
	entities[index].model = model;
	entities[index].material = renderer->GetMaterial(0)->CreateInstances(model);
	return &entities[index++];
}

OPsceneLight* OPscene::Add(OPlightSpot light) {
	lights[lightIndex].light = light;
	lights[lightIndex].world = OPMAT4_IDENTITY;
	lights[lightIndex].world.Translate(light.position);
	lights[lightIndex].world.Scl(light.radius);
	return &lights[lightIndex++];
}

OPint OPscene::Update(OPtimer* timer) {
	return 0;
}

void OPscene::Render(OPfloat delta) {

	renderer->Begin();

	for (ui32 i = 0; i < index; i++) {
		renderer->Submit(entities[i].model, &entities[i].world, entities[i].material);
	}

	for (ui32 i = 0; i < lightIndex; i++) {
		renderer->Submit(&lights[i].light, &lights[i].world);
	}

	renderer->End();

	renderer->Present();

}

void OPscene::Destroy() {
	OPfree(entities);
}