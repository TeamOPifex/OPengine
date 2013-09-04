#pragma once

#include "ShaderAttribute.h"

class ShaderAttributeUV : public ShaderAttribute {
public:
	ShaderAttributeUV(){}
	ShaderAttributeUV(MaterialPtr material): ShaderAttribute() 
	{
		Init(material);
	}

	void Init(MaterialPtr material){
		ShaderAttribute::Init(material, "aUV");
	}
};