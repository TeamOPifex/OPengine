#pragma once
#include "ShaderParamMatrix.h"

class ShaderParamView : public ShaderParamMatrix {
public:
	ShaderParamView(Material* material) : ShaderParamMatrix(material,"mView") { }
	void SetViewMatrix(Matrix4* value) {
		ShaderParamMatrix::SetMatrix(value);
	}
};