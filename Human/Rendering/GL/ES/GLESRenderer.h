#pragma once

#include "Core\Types.h"
#include "../../Renderer.h"

namespace OPifex{
	class GLESRenderer : public Renderer
	{
	public:
		GLESRenderer();
		~GLESRenderer() {
		}
		int initialize();
		void clear_color(f32 r, f32 g, f32 b);
		void use_material(MaterialPtr material);
		void set_viewport(ui32 x, ui32 y, ui32 width, ui32 height);
		void swap_buffer();
		void shutdown();
	};
}