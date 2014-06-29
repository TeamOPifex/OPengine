#include "./Human/include/Rendering/Mesh.h"
#include "./Human/include/Rendering/Effect.h"
#include "./Human/include/Rendering/Camera.h"

inline void OPrenderBindMeshAndEffect(OPmesh* mesh, OPeffect* effect) {
	OPrenderBindMesh(mesh);
	OPrenderBindEffect(effect);
}

inline void OPrenderMesh3D(OPmat4* world, OPcam* cam) {
	OPmat4 view, proj;
	OPcamGetView((*cam), &view);
	OPcamGetProj((*cam), &proj);
	OPrenderParamMat4v("uWorld", 1, world);
	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);
	OPrenderMesh();
}