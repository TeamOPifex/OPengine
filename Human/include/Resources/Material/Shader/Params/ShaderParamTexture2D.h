#pragma once
#include "ShaderParam.h"
#include "./Human/include/Resources/Texture/Texture2D.h"
#include "./Human/include/Resources/Material/Material.h"

class ShaderParamTexture2D : public ShaderParam {
public:
	ShaderParamTexture2D(Material* material, const char* variable) : ShaderParam(material) { 		
		_handle = material->GetUniform(variable);
	}
	void SetTexture(Texture2D* value, int slot) {
		value->Bind(_handle, slot);
	}
};