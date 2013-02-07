#pragma once
#include "ShaderParamTexture2D.h"

class ShaderParamColorTexture : public ShaderParamTexture2D {
public:
	ShaderParamColorTexture(Material* material) : ShaderParamTexture2D(material,"uColorTexture") { }
};