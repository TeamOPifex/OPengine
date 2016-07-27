#include "./Human/include/Rendering/OPmodel.h"

void OPmodel::Bind(OPmaterial* material) {
	mesh->Bind();
	OPmaterialBind(material);
	OPeffectSet("uWorld", &world);
}

void OPmodel::Bind(OPmaterial* material, OPcam* camera) {
	Bind(material);
	OPcamBind(camera);
}

void OPmodel::Draw(OPmaterial* material, OPcam* cam) {
	Bind(material);
	OPeffectSet(cam);
	OPmeshRender();
}