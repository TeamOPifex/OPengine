#pragma once

#include "Core\Types.h"
#include "ApiRenderer.h"

namespace OPifex{
	class GLESRenderer : public ApiRenderer
	{
	public:
		GLESRenderer();
		~GLESRenderer() {
		}
		void clear_color();
	};
}