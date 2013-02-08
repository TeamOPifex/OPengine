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
	RenderSystem::SetBuffer(VertexBuffer, ModelMesh->VertexBuffer->Handle());
	RenderSystem::SetBuffer(IndexBuffer, ModelMesh->IndexBuffer->Handle());
	ModelMaterial->SetMeshData(ModelMesh);
}