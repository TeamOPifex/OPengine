#pragma once

#include "ShaderAttribute.h"

class ShaderAttributePosition : public ShaderAttribute {
public:
	ShaderAttributePosition(MaterialPtr material) : ShaderAttribute(material, "aPosition"){	}
};