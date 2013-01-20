#pragma once
#include "./Core/include/Types.h"
#include "Mesh.h"
#include "Model.h"
#include "./Human/Math/Matrix4.h"
#include "./Human/Rendering/GL/GLWorldTexturedSpecularMaterial.h"

class ModelWorld : public Model {
public:
	ModelWorld(Mesh* mesh, GLWorldMaterial* material);
	~ModelWorld();

	virtual void SetMaterialData();

	Matrix4* WorldMatrix;
};