#ifndef OPENGINE_HUMAN_RENDERING_MODEL
#define OPENGINE_HUMAN_RENDERING_MODEL

#include "OPmesh.h"
#include "OPeffect.h"
#include "OPmaterial.h"
#include "./Math/include/OPmat4.h"

typedef struct {
	OPmaterial material;
	OPmat4 world;
	OPmesh* mesh;
} OPmodel;

inline void OPmodelBind(OPmodel model) {
	OPmeshBind(model.mesh);
	OPmaterialBind(&model.material);
	OPeffectParam("uWorld", model.world);
}

inline void OPmodelDraw(OPmodel model, OPcam cam) {
	OPmodelBind(model);
	OPeffectParam(cam);
	OPmeshRender();
}

#endif