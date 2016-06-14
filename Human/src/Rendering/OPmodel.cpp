#include "./Human/include/Rendering/OPmodel.h"

void OPmodel::Bind(OPmaterial* material) {
	mesh->Bind();
	OPmaterialBind(material);
	OPeffectParam("uWorld", world);
}

void OPmodel::Bind(OPmaterial* material, OPcam* camera) {
	Bind(material);
	OPcamBind(camera);
}

void OPmodel::Draw(OPmaterial* material, OPcam* cam) {
	Bind(material);
	OPeffectParam(cam);
	OPmeshRender();
}