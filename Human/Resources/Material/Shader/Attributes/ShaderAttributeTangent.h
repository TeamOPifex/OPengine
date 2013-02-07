#pragma once

#include "ShaderAttribute.h"

class ShaderAttributeTangent : public ShaderAttribute {
public:
	ShaderAttributeTangent(MaterialPtr material) : ShaderAttribute(material, "aTangent"){	}
};