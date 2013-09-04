#include "./Human/include/Resources/Model/PackedModel.h"
#include "./Human/include/Rendering/RenderSystem.h"

PackedModel::PackedModel(PackedMesh* mesh, Material* material){
	Init(mesh, material);
}

void PackedModel::Init(PackedMesh* mesh, Material* material){
	ModelMesh = mesh;
	ModelMaterial = material;
	WorldMatrix = new Matrix4();
	WorldMatrix->SetIdentity();
}

PackedModel::~PackedModel(){
	delete WorldMatrix;
}

void PackedModel::SetMeshData() {
	RenderSystem::SetBuffer(VertexBuffer, ModelMesh->MeshPackerPtr->VertexBufferPtr->Handle());
	RenderSystem::SetBuffer(IndexBuffer, ModelMesh->MeshPackerPtr->IndexBufferPtr->Handle());
	ModelMaterial->SetMeshData(ModelMesh);
}