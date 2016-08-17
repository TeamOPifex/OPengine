#pragma once

struct OPmodel;
typedef struct OPmodel OPmodel;

struct OPmodelTextured;
typedef struct OPmodelTextured OPmodelTextured;

#include "OPmesh.h"
#include "OPeffect.h"
#include "OPmaterial.h"
#include "./Math/include/OPmat4.h"
#include "./Human/include/Rendering/OPcam.h"
#include "./Data/include/OPcman.h"

struct OPmodel {
	OPmat4 world;
	OPmesh* mesh;

	void Init(const OPchar* mesh) {
	    this->mesh = (OPmesh*)OPCMAN.LoadGet(mesh);
		this->world = OPMAT4_IDENTITY;
	}

	void Bind(OPmaterial* material);
	void Bind(OPmaterial* material, OPcam* camera);
	void Draw(OPmaterial* material, OPcam* camera);
};


// TODO: (garrett) remove this
struct OPmodelTextured {
	OPmodel model;
	OPtexture* texture;

	void Init(const OPchar* modelAsset, const OPchar* textureAsset) {
		model.Init(modelAsset);
		texture = OPtextureLoad(textureAsset);
	}
};