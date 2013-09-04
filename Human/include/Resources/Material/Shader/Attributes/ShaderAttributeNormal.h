#pragma once

#include "ShaderAttribute.h"

class ShaderAttributeNormal : public ShaderAttribute {
public:
	ShaderAttributeNormal() {}
	ShaderAttributeNormal(MaterialPtr material) : ShaderAttribute() 
	{
		Init(material);
	}

	void Init(MaterialPtr material) {
		ShaderAttribute::Init(material, "aNormal");
	}
};