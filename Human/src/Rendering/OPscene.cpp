#include "./Human/include/Rendering/OPscene.h"

void OPscene::Init(OPrenderer2* renderer, ui32 maxEntities, ui32 maxLights) {
	
	index = 0;
	count = maxEntities;
	entities = OPALLOC(OPrendererEntity, count);

	lightIndex = 0;
	lightCount = maxLights;
	lights = OPALLOC(OPsceneLight, lightCount);

	internalCamera.SetPerspective(OPvec3(2), OPvec3(0));
	OPfloat shadowCameraSize = 16.0f;
	internalShadowCamera.SetOrtho(OPvec3(-20, 50, 10), OPVEC3_ZERO, OPVEC3_UP, 25.0f, 150.0f, -shadowCameraSize, shadowCameraSize, -shadowCameraSize, shadowCameraSize);
	camera = &internalCamera;
	shadowCamera = &internalShadowCamera;

	this->renderer = renderer;
	this->renderer->Init(&camera, &shadowCamera);// &camera, maxEntities, 1);
	//this->renderer->SetCamera(&camera);
	//this->renderer->shadowCamera = &shadowCamera;
}

void OPscene::SetCamera(OPcam* cam) {
	camera = cam;
	renderer->SetCamera(&camera);
}

void OPscene::SetShadowCamera(OPcam* cam) {
	shadowCamera = cam;
	renderer->SetShadowCamera(&shadowCamera);
}

OPrendererEntity* OPscene::Add(OPmodel* model, OPmaterial* material, OPrendererEntityDesc desc) {
	ASSERT(index < count, "Hit Max Entities");
	entities[index].world = OPMAT4_IDENTITY;
	entities[index].model = model;
	entities[index].material = material;
	entities[index].desc = desc;
	return &entities[index++];
}

OPrendererEntity* OPscene::Add(OPmodel* model, OPrendererEntityDesc desc) {
	ASSERT(index < count, "Hit Max Entities");
	entities[index].world = OPMAT4_IDENTITY;
	entities[index].model = model;
	entities[index].desc = desc;
	renderer->SetMaterials(&entities[index]);
	OPmaterial::SetMeta(&entities[index]);
	return &entities[index++];
}

OPrendererEntity* OPscene::Add(OPmodel* model, OPskeleton* skeleton, OPrendererEntityDesc desc) {
	ASSERT(index < count, "Hit Max Entities");
	desc.animated = true;
	entities[index].world = OPMAT4_IDENTITY;
	entities[index].model = model;
	entities[index].desc = desc;
	entities[index].shadowMaterial = NULL;
	renderer->SetMaterials(&entities[index]);
	OPmaterial::SetMeta(&entities[index]);
	entities[index].material->AddParam(skeleton);
	if (entities[index].shadowMaterial != NULL) {
		entities[index].shadowMaterial->AddParam(skeleton);
	}
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