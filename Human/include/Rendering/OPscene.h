#pragma once

struct OPscene;
struct OPsceneEntity;

typedef struct OPscene OPscene;
typedef struct OPsceneEntity OPsceneEntity;

#include "./Human/include/Rendering/OPmodel.h"
#include "./Human/include/Rendering/OPrenderCommandBucket.h"
#include "./Human/include/Rendering/OPrenderer.h"
#include "./Human/include/Rendering/OPlightSpot.h"

struct OPsceneEntity {
	OPmat4 world;
	OPmodel* model;
	OPmaterialInstance* material;
};

struct OPsceneLight {
	OPmat4 world;
	OPlightSpot light;
};

struct OPscene {
	OPrenderer* renderer;
	OPsceneEntity* entities;
	ui32 count;
	ui32 index;
	OPsceneLight* lights;
	ui32 lightCount;
	ui32 lightIndex;
	OPcam* camera;
	OPcam internalCamera;

	void Init(OPrenderer* renderer, ui32 maxEntities, ui32 maxLights);
	OPsceneEntity* Add(OPmodel* model);
	OPsceneEntity* Add(OPmodel* model, OPmaterialInstance* material);
	OPsceneLight* Add(OPlightSpot light);
	OPint Update(OPtimer* timer);
	void Render(OPfloat delta);
	void Destroy();
};