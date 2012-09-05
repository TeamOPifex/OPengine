#pragma once

#include "Core\Types.h"
#include "Renderer.h"

namespace OPifex{
	class GLESRenderer : public Renderer
	{
	public:
		GLESRenderer();

		void clear_color(f32 r);
	};
}