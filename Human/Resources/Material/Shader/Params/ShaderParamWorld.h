#pragma once
#include "ShaderParamMatrix.h"

class ShaderParamWorld : public ShaderParamMatrix {
public:
	ShaderParamWorld(Material* material) : ShaderParamMatrix(material, "mWorld") { }
	void SetWorldMatrix(Matrix4* value) {
		ShaderParamMatrix::SetMatrix(value);
	}
};