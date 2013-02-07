#pragma once
#include "ShaderParamMatrix.h"

class ShaderParamProjection : public ShaderParamMatrix {
public:
	ShaderParamProjection(Material* material) : ShaderParamMatrix(material,"uProjection") { }
};