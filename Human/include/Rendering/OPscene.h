#pragma once

struct OPscene;
struct OPsceneEntity;

typedef struct OPscene OPscene;
typedef struct OPsceneEntity OPsceneEntity;

#include "./Human/include/Rendering/OPmodel.h"
#include "./Human/include/Rendering/OPrenderCommandBucket.h"
#include "./Human/include/Rendering/OPrenderer.h"
#include "./Human/include/Rendering/OPlightSpot.h"
#include "./Human/include/Rendering/OPrendererEntity.h"

//struct OPsceneEntity {
//	OPmat4 world;
//	OPmodel* model;
//	OPmaterialInstance** material;
//	bool shadowEmitter;
//	bool shadowReceiver;
//};

struct OPsceneLight {
	OPlightSpot light;
};

struct OPscene {
	OPrenderer2* renderer;
	OPrendererEntity* entities;
	bool* entitiesState;
	ui32 count;
	ui32 index;
	OPsceneLight* lights;
	ui32 lightCount;
	ui32 lightIndex;
	OPcam* camera;
	OPcam* shadowCamera;
	OPcam internalCamera;
	OPcam internalShadowCamera;

	void Init(OPrenderer2* renderer, ui32 maxEntities, ui32 maxLights);
	OPrendererEntity* Add(OPmodel* model, OPrendererEntityDesc desc);
	OPrendererEntity* Add(OPmodel* model, OPmaterial* material, OPrendererEntityDesc desc);
	OPrendererEntity* Add(OPmodel* model, OPskeleton* skeleton, OPrendererEntityDesc desc);
	OPsceneLight* Add(OPlightSpot light);
	void SetCamera(OPcam* cam);
	void SetShadowCamera(OPcam* cam);
	OPrendererEntity* Remove(OPrendererEntity* entity);
	OPint Update(OPtimer* timer);
	void Render(OPfloat delta);
	void Destroy();

	OPrendererEntity* NextEntity();
};


struct OPsceneVR : OPscene {
	OPcam* camera2;
	OPcam internalCamera2;

	void Init(OPrenderer2* renderer, ui32 maxEntities, ui32 maxLights);
	void SetCamera2(OPcam* cam);
	void RenderLeft(OPfloat delta);
	void RenderRight(OPfloat delta);
	void RenderWith(OPcam* cam, OPfloat delta);
};