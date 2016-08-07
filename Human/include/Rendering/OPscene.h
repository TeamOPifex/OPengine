#pragma once

struct OPscene;
struct OPsceneEntity;

#include "./Human/include/Rendering/OPmodel.h"
#include "./Human/include/Rendering/OPrenderCommandBucket.h"
#include "./Human/include/Rendering/OPrenderer.h"

struct OPsceneEntity {
	OPmodel* model;
	OPmaterialInstance* material;
};

struct OPscene {
	OPrenderer* renderer;
	OPsceneEntity* entities;
	ui32 count;
	ui32 index;
	OPcam* camera;
	OPcam internalCamera;

	void Init(OPrenderer* renderer, ui32 maxEntities);
	OPsceneEntity* Add(OPmodel* model);
	OPsceneEntity* Add(OPmodel* model, OPmaterialInstance* material);
	OPint Update(OPtimer* timer);
	void Render(OPfloat delta);
};