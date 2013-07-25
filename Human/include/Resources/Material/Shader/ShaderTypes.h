#pragma once

#include "./Core/include/Types.h"

enum ShaderType{
	Vertex,
	Fragment
};

ui32 GetShaderType(ShaderType shaderType);
ShaderType GetShaderType(ui32 shaderType);
