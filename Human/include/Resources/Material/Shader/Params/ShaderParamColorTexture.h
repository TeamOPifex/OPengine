#pragma once
#include "ShaderParamTexture2D.h"

class ShaderParamColorTexture : public ShaderParamTexture2D {
public:
	ShaderParamColorTexture(){}
	ShaderParamColorTexture(Material* material) : ShaderParamTexture2D() { Init(material); }
	void Init(Material* material) {
		ShaderParamTexture2D::Init(material,"uColorTexture");
	}
};