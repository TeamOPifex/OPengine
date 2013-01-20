#pragma once

#include "GLWorldProjMaterial.h"
#include "./Human/Rendering/GL/GLTexture.h"

class GLWorldTexturedSpecularMaterial : public GLWorldProjMaterial {
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

	void SetTexture(GLTexture* texture, ui32 slot);
	void SetTextureNormal(GLTexture* texture, ui32 slot);
	void SetTextureSpecular(GLTexture* texture, ui32 slot);
	
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