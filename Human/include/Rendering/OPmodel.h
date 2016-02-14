#ifndef OPENGINE_HUMAN_RENDERING_MODEL
#define OPENGINE_HUMAN_RENDERING_MODEL

#include "OPmesh.h"
#include "OPeffect.h"
#include "OPmaterial.h"
#include "./Math/include/OPmat4.h"
#include "./Human/include/Rendering/OPcam.h"
#include "./Data/include/OPcman.h"

struct OPmodel;
void OPmodelBind(OPmodel model, OPmaterial* material);
void OPmodelBind(OPmodel model, OPmaterial* material, OPcam camera);
void OPmodelDraw(OPmodel model, OPmaterial* material, OPcam camera);

struct OPmodel {
	OPmat4 world;
	OPmesh* mesh;
	
	void Init(const OPchar* mesh) {
	    this->mesh = (OPmesh*)OPcmanLoadGet(mesh);
	}
	
	void Bind(OPmaterial* material) {
	    OPmodelBind(*this, material);
	}
	
	void Bind(OPmaterial* material, OPcam camera) {
	    OPmodelBind(*this, material, camera);
	}
	
	void Draw(OPmaterial* material, OPcam camera) {
	    OPmodelDraw(*this, material, camera);
	}
};
typedef struct OPmodel OPmodel;

#endif
