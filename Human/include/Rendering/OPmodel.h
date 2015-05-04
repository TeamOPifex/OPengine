#ifndef OPENGINE_HUMAN_RENDERING_MODEL
#define OPENGINE_HUMAN_RENDERING_MODEL

#include "OPmesh.h"
#include "OPeffect.h"
#include "OPmaterial.h"
#include "./Math/include/OPmat4.h"

typedef struct {
	OPmat4 world;
	OPmesh* mesh;
} OPmodel;

inline void OPmodelBind(OPmodel model, OPmaterial* material) {
	OPmeshBind(model.mesh);
	OPmaterialBind(material);
	OPeffectParam("uWorld", model.world);
}

inline void OPmodelDraw(OPmodel model, OPmaterial* material, OPcam cam) {
	OPmodelBind(model, material);
	OPeffectParam(cam);
	OPmeshRender();
}

#endif