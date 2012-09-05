#pragma once

#include "Core\Types.h"
#include "ApiRenderer.h"

namespace OPifex{
	class Renderer{
	public:
		Renderer(){}
		virtual ~Renderer() { }
		static int Initialize();
		static void ClearColor(f32 r, f32 g, f32 b);
		static void Present();

		 // This will be removed, and is gross
		static bool escape();

		static void Shutdown();
	private:
		static ApiRenderer* m_api_renderer;
	};
}
