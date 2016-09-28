#pragma once

struct OPrendererEntity;
typedef struct OPrendererEntity OPrendererEntity;

struct OPrendererEntityDesc;
typedef struct OPrendererEntityDesc OPrendererEntityDesc;

#include "./Human/include/Rendering/OPcam.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPrenderCommandBucket.h"
#include "./Human/include/Rendering/OPlightSpot.h"

struct OPrendererEntityDesc {
	bool animated = false;
	bool shadowReceiver = true;
	bool shadowEmitter = true;
	bool materialPerMesh = true;

	OPrendererEntityDesc(bool animated) {
		this->animated = animated;
	}

	OPrendererEntityDesc(bool shadowReceiver, bool shadowEmitter) {
		this->shadowReceiver = shadowReceiver;
		this->shadowEmitter = shadowEmitter;
	}

	OPrendererEntityDesc(bool animated, bool shadowReceiver, bool shadowEmitter) {
		this->animated = animated;
		this->shadowReceiver = shadowReceiver;
		this->shadowEmitter = shadowEmitter;
	}

	OPrendererEntityDesc(bool animated, bool shadowReceiver, bool shadowEmitter, bool materialPerMesh) {
		this->animated = animated;
		this->shadowReceiver = shadowReceiver;
		this->shadowEmitter = shadowEmitter;
		this->materialPerMesh = materialPerMesh;
	}
};

struct OPrendererEntity {
	OPmat4 world;
	OPmodel* model;
	OPmaterialInstance** material = NULL;
	OPmaterialInstance** shadowMaterial = NULL;
	OPrendererEntityDesc desc;
};