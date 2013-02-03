#pragma once
#include "ShaderParamMatrix.h"

class ShaderParamProjection : public ShaderParamMatrix {
public:
	ShaderParamProjection(Material* material) : ShaderParamMatrix(material,"mProjection") { }
	void SetProjectionMatrix(Matrix4* value) {
		ShaderParamMatrix::SetMatrix(value);
	}
};