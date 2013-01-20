#pragma once

#include "GLWorldMaterial.h"
#include "./Human/Rendering/GL/GLTexture.h"

class GLTexturedMaterial : public GLWorldMaterial {
public:
	GLTexturedMaterial();
	GLTexturedMaterial(ShaderPtr vertex, ShaderPtr fragment);

	ui32 GetTextureLocation();
	
	void SetPositionData(ui32 stride, void* data);
	void SetUVData(ui32 stride, void* data);
	virtual void SetData(Mesh* mesh);

	void SetTexture(GLTexture* texture, ui32 slot);	

	void EnableAttributes();
	void DisableAttributes();
private:
	ui32 _positionLoc;
	ui32 _uvLoc;
	ui32 _textureLoc;
};