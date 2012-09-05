#pragma once

#include "Core\Types.h"
#include "ApiRenderer.h"

namespace OPifex{
	class Renderer{
	public:
		Renderer(){}
		virtual ~Renderer() { }
		static int Initialize();
		static void clear_color(f32 r, f32 g, f32 b);
		static bool escape();
		static void shutdown();
	private:
		static ApiRenderer* m_api_renderer;
	};
}