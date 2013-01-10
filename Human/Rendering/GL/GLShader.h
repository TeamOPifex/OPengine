#pragma once

#include "./Human/Resources/Material/Shader/Shader.h"
#include "./Human/Resources/Material/Shader/ShaderTypes.h"

	class GLShader : public Shader{
	public:
		GLShader();
		GLShader(ShaderType shaderType, const char* file);
		~GLShader();
		void load(ShaderType shaderType, const char* file);
	};
