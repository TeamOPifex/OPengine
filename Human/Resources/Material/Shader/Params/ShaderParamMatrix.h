#pragma once
#include "ShaderParam.h"
#include "./Human/Math/Matrix4.h"
#include "./Human/Resources/Material/Material.h"

class ShaderParamMatrix : public ShaderParam {
public:
	ShaderParamMatrix(Material* material, const char* variable) : ShaderParam(material) {
		_handle = material->GetUniform(variable);
	}
	void SetMatrix(Matrix4* value) {
		_material->SetMatrix(_handle, value);
	}
};