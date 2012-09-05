#pragma once

#include "Core\Types.h"
#include "../ApiRenderer.h"

namespace OPifex{
	class GLESRenderer : public ApiRenderer
	{
	public:
		GLESRenderer();
		~GLESRenderer() {
		}
		int initialize();
		void clear_color(f32 r, f32 g, f32 b);
		void swap_buffer();
		void shutdown();
	};
}