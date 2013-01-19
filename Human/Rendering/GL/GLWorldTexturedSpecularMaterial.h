#pragma once

#include "GLWorldMaterial.h"
#include "./Human/Rendering/GL/GLTexture.h"

class GLWorldTexturedSpecularMaterial : public GLWorldMaterial {
public:
	GLWorldTexturedSpecularMaterial();
	GLWorldTexturedSpecularMaterial(ShaderPtr vertex, ShaderPtr fragment);

	ui32 GetTextureLocation();
	ui32 GetTextureNormalLocation();
	ui32 GetTextureSpecularLocation();
	
	void SetUVData(ui32 stride, void* data);
	void SetNormalData(ui32 stride, void* data);
	void SetTangentData(ui32 stride, void* data);
	virtual void SetData(Mesh* mesh);

	ui32 SetTexture(GLTexture* texture, ui32 slot);
	ui32 SetTextureNormal(GLTexture* texture, ui32 slot);
	ui32 SetTextureSpecular(GLTexture* texture, ui32 slot);
	
	void EnableAttributes();
	void DisableAttributes();
private:
	ui32 _uvLoc;
	ui32 _normalLoc;
	ui32 _tangentLoc;
	ui32 _textureLoc;
	ui32 _textureNormalLoc;
	ui32 _textureSpecularLoc;
};