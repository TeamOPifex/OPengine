#ifndef OPENGINE_PIPELINE_RENDERING
#define OPENGINE_PIPELINE_RENDERING

#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPcam.h"

inline void OPmeshBindAndEffect(OPmesh* mesh, OPeffect* effect) {
	mesh->Bind();
	OPeffectBind(effect);
}

inline void OPmeshRender3D(OPmat4* world, OPcam* cam) {
	OPeffectParamMat4("uWorld", world);
	OPeffectParamMat4("uProj", &cam->proj);
	OPeffectParamMat4("uView", &cam->view);
	OPmeshRender();
}

inline void OPbindMeshEffectWorldCam(OPmesh* mesh, OPeffect* effect, OPmat4* world, OPcam* camera) {
	mesh->Bind();
	OPeffectBind(effect);

	//OPcamUpdate(camera);

	OPeffectParamMat4("uWorld", world);
	OPeffectParamMat4("uProj", &camera->proj);
	OPeffectParamMat4("uView", &camera->view);
}

#endif
