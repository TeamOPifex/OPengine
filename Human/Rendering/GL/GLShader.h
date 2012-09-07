#pragma once

#include "Data\Resources\Material\Shader\Shader.h"

namespace OPifex{
	class GLShader : public Shader{
		void load(const char* file);
	};
}