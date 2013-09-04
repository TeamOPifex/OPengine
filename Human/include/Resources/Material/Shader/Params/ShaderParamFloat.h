#pragma once
#include "ShaderParam.h"
#include "./Human/include/Math/Matrix4.h"
#include "./Human/include/Resources/Material/Material.h"
#include "./Human/include/Utilities/Errors.h"

class ShaderParamFloat : public ShaderParam {
public:
	ShaderParamFloat(){}
	ShaderParamFloat(Material* material, const char* variable) : ShaderParam(material) {
		_handle = material->GetUniform(variable);
	}
	void Init(Material* material, const char* variable) {
		_handle = material->GetUniform(variable);
		 ShaderParam::Init(material);
	}

	void SetFloat(f32 value) {
		_material->SetFloat(_handle, value);
		CheckError("ShaderParamFloat::Error 1 - Failed to set float");
	}
};