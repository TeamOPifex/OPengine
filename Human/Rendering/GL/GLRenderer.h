#pragma once

#include "./Core/include/Types.h"
#include "../Renderer.h"

class GLRenderer : public Renderer
{
public:
	GLRenderer();
	~GLRenderer() {
	}
	int initialize();
	void clear_color(f32 r, f32 g, f32 b);
	void use_material(MaterialPtr material);
	void set_viewport(ui32 x, ui32 y, ui32 width, ui32 height);
	void set_buffer(ui32 buffer);
	void render_triangles(ui32 offset, ui32 count);
	void swap_buffer();
	void shutdown();
};
