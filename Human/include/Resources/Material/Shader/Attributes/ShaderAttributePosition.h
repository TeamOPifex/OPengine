#pragma once

#include "ShaderAttribute.h"

class ShaderAttributePosition : public ShaderAttribute {
public:
	ShaderAttributePosition(){}
	ShaderAttributePosition(MaterialPtr material) : ShaderAttribute() 
	{
		Init(material);
	}

	void Init(MaterialPtr material){
		ShaderAttribute::Init(material, "aPosition");
	}
};