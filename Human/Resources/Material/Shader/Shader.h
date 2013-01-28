#pragma once

#include "./Human/Resources/Resource.h"
#include "Core/include/Types.h"
#include "ShaderTypes.h"

class Shader : public Resource {
public:
	Shader(ShaderType shaderType, const char* file);
};

typedef Shader* ShaderPtr;