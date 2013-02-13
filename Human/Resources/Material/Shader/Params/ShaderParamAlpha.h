#pragma once
#include "ShaderParamFloat.h"

class ShaderParamAlpha : public ShaderParamFloat {
public:
	ShaderParamAlpha(Material* material) : ShaderParamFloat(material,"uAlpha") { }
};