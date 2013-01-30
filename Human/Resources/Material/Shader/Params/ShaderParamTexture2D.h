#pragma once
#include "ShaderParam.h"
#include "./Human/Resources/Texture/Texture2D.h"
#include "./Human/Resources/Material/Material.h"

class ShaderParamTexture2D : ShaderParam {
public:
	ShaderParamTexture2D(Material* material, const char* variable) { 		
		//_handle = glGetAttribLocation(program, variable);
	}
protected:
	void SetTexture(Texture2D* value) {
		
	}
};