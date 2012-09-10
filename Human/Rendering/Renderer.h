#pragma once

#include "Core\Types.h"
#include "Data\Resources\Material\Material.h"

namespace OPifex{
	class Renderer{
	public:
		Renderer(){}
		virtual ~Renderer() { }
		virtual int initialize() = 0;
		virtual void clear_color(f32 r, f32 g, f32 b) = 0;
		virtual void use_material(MaterialPtr material) = 0;
		virtual void set_viewport(ui32 x, ui32 y, ui32 width, ui32 height) = 0;
		virtual void set_buffer(ui32 buffer) = 0;
		virtual void render_triangles(ui32 offset, ui32 count) = 0;
		virtual void swap_buffer() = 0;
		virtual void shutdown() = 0;
	};
}
