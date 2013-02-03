#pragma once
#include "ShaderParamTexture2D.h"

class ShaderParamColorTexture : public ShaderParamTexture2D {
public:
	ShaderParamColorTexture(Material* material) : ShaderParamTexture2D(material,"tColor") { }
	void SetColorTexture(Texture2D* texture){
		ShaderParamTexture2D::SetTexture(texture);
	}
};