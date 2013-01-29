#pragma once
#include "./Core/include/Types.h"
#include "Mesh.h"
#include "./Human/Math/Matrix4.h"
#include "./Human/Resources/Material/Material.h"

class Model{
public:
	Model(MeshPtr mesh, MaterialPtr material);
	~Model();

	MeshPtr ModelMesh;
	MaterialPtr ModelMaterial;
	Matrix4* WorldMatrix;
};

typedef Model* ModelPtr; 