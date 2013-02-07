#pragma once

#include "ShaderAttribute.h"

class ShaderAttributeUV : public ShaderAttribute {
public:
	ShaderAttributeUV(MaterialPtr material) : ShaderAttribute(material, "aUV"){	}
};