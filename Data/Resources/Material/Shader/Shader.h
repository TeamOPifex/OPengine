#pragma once

#include "Core/Types.h"

namespace OPifex{
	class Shader {
	public:
		virtual void load(const char* file) = 0;
		i32 handle(){ return ptr; }
	private:
		i32 ptr;
	};
}
