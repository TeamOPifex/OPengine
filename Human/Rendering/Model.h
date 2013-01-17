#pragma once
#include "./Core/include/Types.h"
#include "Mesh.h"
#include "./Human/Math/Matrix4.h"
#include "./Human/Rendering/GL/GLWorldMaterial.h"

class Model{
public:
	Model(Mesh* mesh, GLWorldMaterial* material, Matrix4* world){
		ModelMesh = mesh;
		ModelMaterial = material;
		WorldMatrix = world;
	}
	Mesh* ModelMesh;
	GLWorldMaterial* ModelMaterial;
	Matrix4* WorldMatrix;
};