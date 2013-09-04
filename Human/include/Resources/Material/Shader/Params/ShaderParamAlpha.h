#pragma once
#include "ShaderParamFloat.h"

class ShaderParamAlpha : public ShaderParamFloat {
public:
	ShaderParamAlpha(){}
	ShaderParamAlpha(Material* material) : ShaderParamFloat() { Init(material); }
	void Init(Material* material) {
		ShaderParamFloat::Init(material, "uAlpha");
	}
};