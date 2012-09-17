#pragma once

#include "Core/Types.h"
#include "ShaderTypes.h"

	class Shader {
	public:
		virtual void load(ShaderType shaderType, const char* file) = 0;
		ui32 handle(){ return ptr; }
	protected:
		ui32 ptr;
	};
	typedef Shader* ShaderPtr;