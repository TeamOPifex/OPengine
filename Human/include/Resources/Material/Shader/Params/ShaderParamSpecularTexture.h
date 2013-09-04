#pragma once
#include "ShaderParamTexture2D.h"

class ShaderParamSpecularTexture : public ShaderParamTexture2D {
public:
	ShaderParamSpecularTexture(){}
	ShaderParamSpecularTexture(Material* material) : ShaderParamTexture2D() { Init(material); }
	void Init(Material* material) {
		ShaderParamTexture2D::Init(material,"uSpecularTexture");
	}
};