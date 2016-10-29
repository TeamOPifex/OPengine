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
	OPrenderer* renderer;
	OPrendererEntity* entities;
	ui32 count;
	ui32 index;
	OPsceneLight* lights;
	ui32 lightCount;
	ui32 lightIndex;
	OPcam* camera;
	OPcam internalCamera;

	void Init(OPrenderer* renderer, ui32 maxEntities, ui32 maxLights);
	OPrendererEntity* Add(OPmodel* model, OPrendererEntityDesc desc);
	OPrendererEntity* Add(OPmodel* model, OPmaterialInstance** material, OPrendererEntityDesc desc);
	OPrendererEntity* Add(OPmodel* model, OPskeleton* skeleton, OPrendererEntityDesc desc);
	OPsceneLight* Add(OPlightSpot light);
	void Remove(OPrendererEntity* entity);
	OPint Update(OPtimer* timer);
	void Render(OPfloat delta);
	void Destroy();

	inline OPrendererEntity* Add(OPmodel* model, OPmaterialInstance* material, OPrendererEntityDesc desc) {
		desc.materialPerMesh = false;
		return Add(model, (OPmaterialInstance**)material, desc);
	}
};