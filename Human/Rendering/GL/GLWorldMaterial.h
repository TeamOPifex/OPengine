#pragma once

#include "GLMaterial.h"

class GLWorldMaterial : public GLMaterial {
public:
	GLWorldMaterial();
	GLWorldMaterial(ShaderPtr vertex, ShaderPtr fragment);
	void SetWorldMatrix(f32* world);
	void SetViewProjectionMatrix(f32* world);
		
	void SetPositionData(ui32 stride, void* data);
protected:
	ui32 _worldMatrix;
	ui32 _viewProjectionMatrix;
	ui32 _positionLoc;
};