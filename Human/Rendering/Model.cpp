#include "Model.h"
#include "RenderSystem.h"

Model::Model(Mesh* mesh, GLMaterial* material){
	ModelMesh = mesh;
	ModelMaterial = material;
	
	RenderSystem::SetBuffer(1, mesh->VertexBuffer->handle());
	material->SetData(mesh);
}

Model::~Model(){
}

void Model::SetMaterialData(){
	
}