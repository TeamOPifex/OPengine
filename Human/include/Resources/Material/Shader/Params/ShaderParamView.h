#pragma once
#include "ShaderParamMatrix.h"

class ShaderParamView : public ShaderParamMatrix {
public:
	ShaderParamView(){}
	ShaderParamView(Material* material) : ShaderParamMatrix() { Init(material); }
	void Init(Material* material) {
		ShaderParamMatrix::Init(material,"uView");
	}
};