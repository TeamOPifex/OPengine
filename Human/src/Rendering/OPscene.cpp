#include "./Human/include/Rendering/OPscene.h"

void OPscene::Init(OPrenderer2* renderer, ui32 maxEntities, ui32 maxLights) {
	
	index = 0;
	count = maxEntities;
	entities = OPALLOC(OPrendererEntity, count);

	lightIndex = 0;
	lightCount = maxLights;
	lights = OPALLOC(OPsceneLight, lightCount);

	internalCamera.SetPerspective(OPvec3(2), OPvec3(0));
	internalShadowCamera.SetPerspective(OPvec3(2), OPvec3(0));
	camera = &internalCamera;
	shadowCamera = &internalShadowCamera;

	this->renderer = renderer;
	this->renderer->Init();// &camera, maxEntities, 1);
	this->renderer->SetCamera(&camera);
	this->renderer->shadowCamera = &shadowCamera;
}

OPrendererEntity* OPscene::Add(OPmodel* model, OPmaterial* material, OPrendererEntityDesc desc) {
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
	//renderer->SetMaterials(&entities[index]);
	return &entities[index++];
}

OPrendererEntity* OPscene::Add(OPmodel* model, OPskeleton* skeleton, OPrendererEntityDesc desc) {
	desc.animated = true;
	entities[index].world = OPMAT4_IDENTITY;
	entities[index].model = model;
	entities[index].desc = desc;
	renderer->GetMaterial()->CreateInstances(&entities[index]);
	//entities[index].material = material->Create(material);

	//renderer->SetMaterials(&entities[index], skeleton);
	return &entities[index++];
}

OPsceneLight* OPscene::Add(OPlightSpot light) {
	lights[lightIndex].light = light;
	return &lights[lightIndex++];
}

OPrendererEntity* OPscene::Remove(OPrendererEntity* entity) {
	ui32 i = 0;
	bool found = false;
	for (; i < index; i++) {
		if (&entities[i] == entity) {
			found = true;
			break;
		}
	}
	if (found) {
		// TODO: (garrett) This won't work, because of the Ptr's
		if (i != index - 1) {
			entities[i] = entities[index - 1];
		}
		index--;
		return &entities[index];
	}
	return NULL;
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
		//renderer->Submit(&lights[i].light);
	}

	renderer->End();

	renderer->Present();

}

void OPscene::Destroy() {
	OPfree(entities);
}