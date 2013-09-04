#pragma once
#include "ShaderParamMatrix.h"

class ShaderParamProjection : public ShaderParamMatrix {
public:
	ShaderParamProjection(){}
	ShaderParamProjection(Material* material) : ShaderParamMatrix() { Init(material); }
	void Init(Material* material) {
		ShaderParamMatrix::Init(material,"uProjection");
	}
};