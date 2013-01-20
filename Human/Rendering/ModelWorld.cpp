#include "ModelWorld.h"
#include "RenderSystem.h"

ModelWorld::ModelWorld(Mesh* mesh, GLWorldMaterial* material) : Model(mesh, material) {
	WorldMatrix = new Matrix4();
	WorldMatrix->SetIdentity();
}

ModelWorld::~ModelWorld(){
	delete WorldMatrix;
}

void ModelWorld::SetMaterialData(){
	((GLWorldMaterial*)ModelMaterial)->SetWorldMatrix(&((*WorldMatrix)[0][0]));
}