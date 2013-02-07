#pragma once
#include "ShaderParamMatrix.h"

class ShaderParamWorld : public ShaderParamMatrix {
public:
	ShaderParamWorld(Material* material) : ShaderParamMatrix(material, "uWorld") { }
};