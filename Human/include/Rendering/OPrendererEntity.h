#pragma once

struct OPrendererEntity;
typedef struct OPrendererEntity OPrendererEntity;

#include "./Human/include/Rendering/OPcam.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPrenderCommandBucket.h"
#include "./Human/include/Rendering/OPlightSpot.h"

struct OPrendererEntity {
	OPmat4 world;
	OPmodel* model;
	bool materialPerMesh;
	OPmaterialInstance** material;
	OPmaterialInstance** shadowMaterial;
	bool shadowReceiver = true;
	bool shadowEmitter = true;
};