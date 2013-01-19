#pragma once
#include "./Core/include/Types.h"
#include "Mesh.h"
#include "./Human/Math/Matrix4.h"
#include "./Human/Rendering/GL/GLWorldTexturedSpecularMaterial.h"

class Model{
public:
	Model(Mesh* mesh, GLWorldTexturedSpecularMaterial* material);
	~Model();
	Mesh* ModelMesh;
	GLWorldTexturedSpecularMaterial* ModelMaterial;
	Matrix4* WorldMatrix;
};