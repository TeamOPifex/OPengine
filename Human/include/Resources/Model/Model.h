#pragma once
#include "./Core/include/Types.h"
#include "Mesh.h"
#include "./Math/include/Matrix4.h"
#include "./Human/include/Resources/Material/Material.h"

class Model{
public:
	Model();
	Model(MeshPtr mesh, MaterialPtr material);
	~Model();
	void SetMeshData();

	MeshPtr ModelMesh;
	MaterialPtr ModelMaterial;
	Matrix4* WorldMatrix;
};

typedef Model* ModelPtr; 