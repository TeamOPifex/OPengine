#pragma once
#include "ShaderParamTexture2D.h"

class ShaderParamNormalTexture : public ShaderParamTexture2D {
public:
	ShaderParamNormalTexture(){}
	ShaderParamNormalTexture(Material* material) : ShaderParamTexture2D() { Init(material); }
	void Init(Material* material) {
		ShaderParamTexture2D::Init(material,"uNormalTexture");
	}
};