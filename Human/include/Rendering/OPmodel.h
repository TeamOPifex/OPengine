#ifndef OPENGINE_HUMAN_RENDERING_MODEL
#define OPENGINE_HUMAN_RENDERING_MODEL

#include "OPmesh.h"
#include "OPeffect.h"
#include "OPmaterial.h"
#include "./Math/include/OPmat4.h"
#include "./Human/include/Rendering/OPcam.h"
#include "./Data/include/OPcman.h"

struct OPmodel;
typedef struct OPmodel OPmodel;

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


struct OPmodelTextured;
typedef struct OPmodelTextured OPmodelTextured;

struct OPmodelTextured {
	OPmodel model;
	OPtexture* texture;

	void Init(const OPchar* modelAsset, const OPchar* textureAsset) {
		model.Init(modelAsset);
		texture = OPtextureLoad(textureAsset);
	}
};

#endif
