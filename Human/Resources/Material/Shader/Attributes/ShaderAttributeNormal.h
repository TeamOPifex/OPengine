#pragma once

#include "ShaderAttribute.h"

class ShaderAttributeNormal : public ShaderAttribute {
public:
	ShaderAttributeNormal(MaterialPtr material) : ShaderAttribute(material, "aNormal"){	}
};