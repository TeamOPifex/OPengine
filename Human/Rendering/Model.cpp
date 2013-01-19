#include "Model.h"
#include "RenderSystem.h"

Model::Model(Mesh* mesh, GLWorldMaterial* material){
	ModelMesh = mesh;
	ModelMaterial = material;
	WorldMatrix = new Matrix4();
	WorldMatrix->SetIdentity();
	
	RenderSystem::SetBuffer(1, mesh->VertexBuffer->handle());
	material->SetData(mesh);
}

Model::~Model(){
	delete WorldMatrix;
}