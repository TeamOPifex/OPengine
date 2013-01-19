#pragma once
#include "./Core/include/Types.h"
#include "Mesh.h"
#include "./Human/Math/Matrix4.h"
#include "./Human/Rendering/GL/GLWorldTexturedSpecularMaterial.h"

class Model{
public:
	Model(Mesh* mesh, GLWorldMaterial* material);
	~Model();
	Mesh* ModelMesh;
	GLWorldMaterial* ModelMaterial;
	Matrix4* WorldMatrix;
};