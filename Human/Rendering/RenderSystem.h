#pragma once

#include "./Core/include/Types.h"
#include "Renderer.h"
#include "RendererTypes.h"

	class RenderSystem{
	public:
		RenderSystem(){}
		virtual ~RenderSystem() { }
		static int Initialize();
		static void ClearColor(f32 r, f32 g, f32 b);
		static void UseMaterial(MaterialPtr material);
		static void SetViewport(ui32 x, ui32 y, ui32 width, ui32 height);
		static void SetBuffer(int bufferType, ui32 buffer);
		static void RenderTriangles(ui32 offset, ui32 count);
		static void RenderTriangles(ui32 numIndices);
		static void Present();

		 // This will be removed, and is gross
		static bool escape();

		static void Shutdown();
	private:
		static Renderer* m_renderer;
	};
