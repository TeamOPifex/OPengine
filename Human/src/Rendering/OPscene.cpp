#include "./Human/include/Rendering/OPscene.h"

void OPscene::Init(OPrenderer* renderer, ui32 maxEntities) {
	
	index = 0;
	count = maxEntities;
	entities = OPALLOC(OPsceneEntity, count);

	internalCamera.SetPerspective(OPvec3(2), OPvec3(0));
	camera = &internalCamera;

	this->renderer = renderer;
	this->renderer->Init(&camera, maxEntities, 1);
}

OPsceneEntity* OPscene::Add(OPmodel* model, OPmaterialInstance* material) {
	entities[index].model = model;
	entities[index].material = material;
	return &entities[index++];
}

OPsceneEntity* OPscene::Add(OPmodel* model) {
	entities[index].model = model;
	entities[index].material = renderer->CreateMaterialInstance(0);
	return &entities[index++];
}

OPint OPscene::Update(OPtimer* timer) {
	return 0;
}

void OPscene::Render(OPfloat delta) {

	renderer->Begin();

	for (ui32 i = 0; i < index; i++) {
		renderer->Submit(entities[i].model, &entities[i].world, entities[i].material);
	}

	renderer->End();

	renderer->Present();
}

void OPscene::Destroy() {
	OPfree(entities);
}