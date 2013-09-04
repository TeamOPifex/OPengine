#pragma once
#include "ShaderParamMatrix.h"

class ShaderParamWorld : public ShaderParamMatrix {
public:
	ShaderParamWorld(){}
	ShaderParamWorld(Material* material) : ShaderParamMatrix() { Init(material); }
	void Init(Material* material) {
		ShaderParamMatrix::Init(material, "uWorld");
	}
};