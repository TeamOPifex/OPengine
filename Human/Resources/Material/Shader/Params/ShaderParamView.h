#pragma once
#include "ShaderParamMatrix.h"

class ShaderParamView : public ShaderParamMatrix {
public:
	ShaderParamView(Material* material) : ShaderParamMatrix(material,"uView") { }
};