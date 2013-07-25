#pragma once
#include "ShaderParamTexture2D.h"

class ShaderParamSpecularTexture : public ShaderParamTexture2D {
public:
	ShaderParamSpecularTexture(Material* material) : ShaderParamTexture2D(material,"uSpecularTexture") { }
};