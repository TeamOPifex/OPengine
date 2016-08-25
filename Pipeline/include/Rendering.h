#pragma once

#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPcam.h"

inline void OPmeshBindAndEffect(OPmesh* mesh, OPeffect* effect) {
	mesh->Bind();
	effect->Bind();
}

inline void OPmeshRender3D(OPmat4* world, OPcam* cam) {
	OPeffectSet("uWorld", world);
	OPeffectSet("uProj", &cam->proj);
	OPeffectSet("uView", &cam->view);
	OPrenderDrawBufferIndexed(0);
}

inline void OPbindMeshEffectWorldCam(OPmodel* mesh, OPeffect* effect, OPmat4* world, OPcam* camera) {
	effect->Bind();
	OPeffectSet("uWorld", world);
	OPeffectSet("uProj", &camera->proj);
	OPeffectSet("uView", &camera->view);

	mesh->Bind();
}