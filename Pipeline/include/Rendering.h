#ifndef OPENGINE_PIPELINE_RENDERING
#define OPENGINE_PIPELINE_RENDERING

#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPcam.h"

inline void OPrenderBindMeshAndEffect(OPmesh* mesh, OPeffect* effect) {
	OPrenderBindMesh(mesh);
	OPrenderBindEffect(effect);
}

inline void OPrenderMesh3D(OPmat4* world, OPcam* cam) {
	OPmat4 view, proj;
	OPcamGetView((*cam), &view);
	OPcamGetProj((*cam), &proj);
	OPrenderParamMat4("uWorld", world);
	OPrenderParamMat4("uProj", &proj);
	OPrenderParamMat4("uView", &view);
	OPrenderMesh();
}

inline void OPbindMeshEffectWorldCam(OPmesh* mesh, OPeffect* effect, OPmat4* world, OPcam* camera) {
	OPrenderBindMesh(mesh);
	OPrenderBindEffect(effect);

	OPcamUpdateView((*camera));
	OPcamUpdateProj((*camera));

	OPrenderParamMat4("uWorld", world);
	OPrenderParamMat4("uProj", &camera->Proj);
	OPrenderParamMat4("uView", &camera->View);
}

#endif