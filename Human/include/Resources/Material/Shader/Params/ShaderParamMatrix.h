#pragma once
#include "ShaderParam.h"
#include "./Human/include/Math/Matrix4.h"
#include "./Human/include/Resources/Material/Material.h"
#include "./Human/include/Utilities/Errors.h"

class ShaderParamMatrix : public ShaderParam {
public:
	ShaderParamMatrix(Material* material, const char* variable) : ShaderParam(material) {
		_handle = material->GetUniform(variable);
	}
	void SetMatrix(Matrix4* value) {
		_material->SetMatrix(_handle, value);
		//CheckError("ShaderParamMatrix::Error 1 - Failed to set matrix");
	}
};