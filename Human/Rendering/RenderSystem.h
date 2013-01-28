#pragma once

#include "./Core/include/Types.h"
#include "Renderer.h"
#include "RendererTypes.h"
#include "./Human/Resources/Model/Model.h"

class RenderSystem{
public:
	RenderSystem(){}
	virtual ~RenderSystem() { }
	static int Initialize(ui32 width, ui32 height);
	static void ClearColor(f32 r, f32 g, f32 b);
	static void UseMaterial(MaterialPtr material);
	static void SetViewport(ui32 x, ui32 y, ui32 width, ui32 height);
	static void SetBuffer(int bufferType, ui32 buffer);
	static void RenderTriangles(ui32 offset, ui32 count);
	static void RenderTriangles(ui32 numIndices);
	static void RenderModel(Model* model);
	static void Present();
	static void DepthTest(bool state);

	// This will be removed, and is gross
	static bool escape();

	static void Shutdown();
private:
	static Renderer* m_renderer;
};
