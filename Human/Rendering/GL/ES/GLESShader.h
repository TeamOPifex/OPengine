#pragma once

#include "Data\Resources\Material\Shader\Shader.h"
#include "Data\Resources\Material\Shader\ShaderTypes.h"

namespace OPifex{
	class GLESShader : public Shader{
	public:
		GLESShader();
		GLESShader(ShaderType shaderType, const char* file);
		~GLESShader();
		void load(ShaderType shaderType, const char* file);
	};
}