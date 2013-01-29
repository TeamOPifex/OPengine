#include "Model.h"

Model::Model(Mesh* mesh, Material* material){
	ModelMesh = mesh;
	ModelMaterial = material;
}

Model::~Model(){

}