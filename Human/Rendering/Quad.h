#pragma once

#include "Model.h"

#include ".\Core\include\DynamicMemory.h"
#include "./Human/Math/Vector3.h"
#include "./Human/Math/Vector2.h"

#include "./Human/Rendering/GL/GLBuffer.h"

#include "./Human/Rendering/GL/GLWorldMaterial.h"

#include "./Human/Rendering/ModelWorld.h"

class Quad : public ModelWorld {
public:
	Quad(GLWorldMaterial* material);
	~Quad();

	void SetPosition(Vector3* position);
	void SetRotation(Vector3* rotation);	
	void SetScale(Vector3* scale);
	void SetScale(f32 scale);
	
	Vector3* GetPosition();
	Vector3* GetRotation();
	Vector3* GetScale();

	virtual void SetMaterialData();
	
private:
	Mesh* GenMesh();
	void UpdateWorld();

protected:
	Vector3 _position;
	Vector3 _rotation;
	Vector3 _scale;
};