#include "Model.h"
#include "RenderSystem.h"

Model::Model(Mesh* mesh, GLWorldTexturedSpecularMaterial* material){
	ModelMesh = mesh;
	ModelMaterial = material;
	WorldMatrix = new Matrix4();
	WorldMatrix->SetIdentity();
	
	RenderSystem::SetBuffer(1, mesh->VertexBuffer->handle());
	material->SetPositionData(mesh->Stride, (void*)0);
	material->SetUVData(mesh->Stride, (void*)12);
	material->SetNormalData(mesh->Stride, (void*)20);
	material->SetTangentData(mesh->Stride, (void*)32);
}

Model::~Model(){
	delete WorldMatrix;
}