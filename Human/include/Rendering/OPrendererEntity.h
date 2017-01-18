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
	OPmaterial** material = NULL;
	OPmaterial** shadowMaterial = NULL;
	OPrendererEntityDesc desc;

	inline void SetAlbedoMap(const OPchar* texture) {
		if (desc.materialPerMesh) {
			for (ui32 i = 0; i < model->meshCount; i++) {
				material[i]->AddParam("uAlbedoMap", (OPtexture*)OPCMAN.LoadGet(texture), 0);
			}
		}
		else {
			material[0]->AddParam("uAlbedoMap", (OPtexture*)OPCMAN.LoadGet(texture), 0);
		}
	}

	inline void SetAlbedoMap(OPtexture* texture) {
		if (desc.materialPerMesh) {
			for (ui32 i = 0; i < model->meshCount; i++) {
				material[i]->AddParam("uAlbedoMap", texture, 0);
			}
		}
		else {
			material[0]->AddParam("uAlbedoMap", texture, 0);
		}
	}

	inline void SetAlbedoMap(const OPchar* texture, ui32 ind) {
		material[ind]->AddParam("uAlbedoMap", (OPtexture*)OPCMAN.LoadGet(texture), 0);
	}

	inline void SetAlbedoMap(OPtexture* texture, ui32 ind) {
		material[ind]->AddParam("uAlbedoMap", texture, 0);
	}
};