#pragma once

#include "GLMaterial.h"
#include "./Human/Rendering/Mesh.h"

class GLWorldMaterial : public GLMaterial {
public:
	GLWorldMaterial();
	GLWorldMaterial(ShaderPtr vertex, ShaderPtr fragment);
	void SetWorldMatrix(f32* world);
	void SetViewProjectionMatrix(f32* world);
		
	void SetPositionData(ui32 stride, void* data);
	virtual void SetData(Mesh* mesh);
protected:
	ui32 _worldMatrix;
	ui32 _viewProjectionMatrix;
	ui32 _positionLoc;
};