#include "Model.h"
#include "./Human/Rendering/RenderSystem.h"

Model::Model(Mesh* mesh, Material* material){
	ModelMesh = mesh;
	ModelMaterial = material;
	WorldMatrix = new Matrix4();
	WorldMatrix->SetIdentity();
}

Model::~Model(){
	delete WorldMatrix;
}

void Model::SetMeshData() {
	RenderSystem::SetBuffer(1, ModelMesh->VertexBuffer->Handle());
	RenderSystem::SetBuffer(2, ModelMesh->IndexBuffer->Handle());
	ModelMaterial->SetMeshData(ModelMesh);
}