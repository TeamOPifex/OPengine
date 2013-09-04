#pragma once

#include "ShaderAttribute.h"

class ShaderAttributeTangent : public ShaderAttribute {
public:
	ShaderAttributeTangent(){}
	ShaderAttributeTangent(MaterialPtr material): ShaderAttribute() 
	{
		Init(material);
	}

	void Init(MaterialPtr material){
		ShaderAttribute::Init(material, "aTangent");
	}
};