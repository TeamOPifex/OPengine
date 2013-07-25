#pragma once
#include "ShaderParamTexture2D.h"

class ShaderParamNormalTexture : public ShaderParamTexture2D {
public:
	ShaderParamNormalTexture(Material* material) : ShaderParamTexture2D(material,"uNormalTexture") { }
};