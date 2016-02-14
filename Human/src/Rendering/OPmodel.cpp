#include "./Human/include/Rendering/OPmodel.h"

void OPmodelBind(OPmodel model, OPmaterial* material) {
	OPmeshBind(model.mesh);
	OPmaterialBind(material);
	OPeffectParam("uWorld", model.world);
}

void OPmodelBind(OPmodel model, OPmaterial* material, OPcam camera) {
	OPmodelBind(model, material);
	OPcamBind(camera);
}

void OPmodelDraw(OPmodel model, OPmaterial* material, OPcam cam) {
	OPmodelBind(model, material);
	OPeffectParam(cam);
	OPmeshRender();
}