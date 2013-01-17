#pragma once

#include "./Human/Resources/Material/WorldMaterial.h"
#include "GLMaterial.h"

class GLWorldMaterial : public GLMaterial {
public:
	GLWorldMaterial() { }
	GLWorldMaterial(ShaderPtr vertex, ShaderPtr fragment) { 
		load(vertex, fragment); 
		_worldMatrix = attribute_location("Model");
	}
	void SetWorldMatrix(f32* world) { set_matrix(_worldMatrix, world); }
private:
	ui32 _worldMatrix;
};