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

OPrendererEntity* OPscene::Add(OPmodel* model, OPmaterialInstance** material, OPrendererEntityDesc desc) {
	entities[index].world = OPMAT4_IDENTITY;
	entities[index].model = model;
	entities[index].material = material;
	entities[index].desc = desc;
	return &entities[index++];
}

OPrendererEntity* OPscene::Add(OPmodel* model, OPrendererEntityDesc desc) {
	entities[index].world = OPMAT4_IDENTITY;
	entities[index].model = model;
	entities[index].desc = desc;
	renderer->SetMaterials(&entities[index]);
	return &entities[index++];
}

OPrendererEntity* OPscene::Add(OPmodel* model, OPskeleton* skeleton, OPrendererEntityDesc desc) {
	desc.animated = true;
	entities[index].world = OPMAT4_IDENTITY;
	entities[index].model = model;
	entities[index].desc = desc;
	renderer->SetMaterials(&entities[index], skeleton);
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