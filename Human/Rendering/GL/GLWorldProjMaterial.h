#pragma once

#include "GLWorldMaterial.h"
#include "./Human/Rendering/Mesh.h"

class GLWorldProjMaterial : public GLWorldMaterial {
public:
	GLWorldProjMaterial();
	GLWorldProjMaterial(ShaderPtr vertex, ShaderPtr fragment);
	void SetViewProjectionMatrix(f32* world);
		
	void SetPositionData(ui32 stride, void* data);
	virtual void SetData(Mesh* mesh);
protected:
	ui32 _viewProjectionMatrix;
	ui32 _positionLoc;
};