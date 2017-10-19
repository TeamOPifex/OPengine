#include "./Human/include/Rendering/OPscene.h"

void OPscene::Init(OPrenderer2* renderer, ui32 maxEntities, ui32 maxLights) {
	
	index = 0;
	count = maxEntities;
	entities = OPALLOC(OPrendererEntity, count);
	entitiesState = OPALLOC(bool, count);
	for (ui32 i = 0; i < count; i++) {
		entitiesState[i] = false;
	}

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


OPrendererEntity* OPscene::NextEntity() {
	for (ui32 i = 0; i < count; i++) {
		if (entitiesState[i] == false) {
			if (i >= index) {
				index = i + 1;
			}
			entitiesState[i] = true;
			return &entities[i];
		}
	}
	return NULL;
}

OPrendererEntity* OPscene::Add(OPmodel* model, OPmaterial* material, OPrendererEntityDesc desc) {
	ASSERT(index < count, "Hit Max Entities");
	OPrendererEntity* entity = NextEntity();
	entity->world = OPMAT4_IDENTITY;
	entity->model = model;
	entity->material = material;
	entity->desc = desc;
	return entity;
}

OPrendererEntity* OPscene::Add(OPmodel* model, OPrendererEntityDesc desc) {
	ASSERT(index < count, "Hit Max Entities");
	OPrendererEntity* entity = NextEntity();
	entity->world = OPMAT4_IDENTITY;
	entity->model = model;
	entity->desc = desc;
	renderer->SetMaterials(entity);
	OPmaterial::SetMeta(entity);
	return entity;
}

OPrendererEntity* OPscene::Add(OPmodel* model, OPskeleton* skeleton, OPrendererEntityDesc desc) {
	ASSERT(index < count, "Hit Max Entities");
	OPrendererEntity* entity = NextEntity();
	desc.animated = true;
	entity->world = OPMAT4_IDENTITY;
	entity->model = model;
	entity->desc = desc;
	entity->shadowMaterial = NULL;
	renderer->SetMaterials(entity);
	OPmaterial::SetMeta(entity);
	entity->material->AddParam(skeleton);
	if (entity->shadowMaterial != NULL) {
		entity->shadowMaterial->AddParam(skeleton);
	}
	return entity;
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
			entitiesState[i] = false;
			found = true;
			break;
		}
	}
	//if (found) {
	//	// TODO: (garrett) This won't work, because of the Ptr's
	//	if (i != index - 1) {
	//		entities[i] = entities[index - 1];
	//	}
	//	index--;
	//	return &entities[index];
	//}
	return NULL;
}

OPint OPscene::Update(OPtimer* timer) {
	return 0;
}

void OPscene::Render(OPfloat delta) {

	renderer->Begin();

	for (ui32 i = 0; i < index; i++) {
		if(entitiesState[i])
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




void OPsceneVR::Init(OPrenderer2* renderer, ui32 maxEntities, ui32 maxLights) {

	OPscene::Init(renderer, maxEntities, maxLights);
	internalCamera2.SetPerspective(OPvec3(2), OPvec3(0));
	camera2 = &internalCamera;
}

void OPsceneVR::SetCamera2(OPcam* cam) {
	camera2 = cam;
	renderer->SetCamera(&camera);
}

void OPsceneVR::RenderLeft(OPfloat delta) {
	renderer->SetCamera(&camera);
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

void OPsceneVR::RenderRight(OPfloat delta) {
	renderer->SetCamera(&camera2);
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

void OPsceneVR::RenderWith(OPcam* cam, OPfloat delta) {
	renderer->SetCamera(&cam);
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