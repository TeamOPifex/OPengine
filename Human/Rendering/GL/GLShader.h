#pragma once

#include "Data\Resources\Material\Shader\Shader.h"
#include "Data\Resources\Material\Shader\ShaderTypes.h"

namespace OPifex{
	class GLShader : public Shader{
	public:
		GLShader();
		GLShader(ShaderType shaderType, const char* file);
		~GLShader();
		void load(ShaderType shaderType, const char* file);
	};
}