#pragma once
#include "./Core/include/Types.h"
#include "PackedMesh.h"
#include "./Math/include/Matrix4.h"
#include "./Human/include/Resources/Material/Material.h"

class PackedModel{
public:
	PackedModel(PackedMeshPtr mesh, MaterialPtr material);
	~PackedModel();
	void SetMeshData();

	PackedMeshPtr ModelMesh;
	MaterialPtr ModelMaterial;
	OPmat4* WorldMatrix;
};

typedef PackedModel* PackedModelPtr; 