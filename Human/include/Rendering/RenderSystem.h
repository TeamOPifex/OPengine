#pragma once

#include "./Core/include/Types.h"
#include "Renderer.h"
#include "RendererTypes.h"
#include "./Human/include/Resources/Model/Model.h"
#include "./Human/include/Resources/Model/PackedModel.h"

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
	static void RenderModel(PackedModel* model);
	static void Present();
	static void DepthTest(bool state);

	static i32 ScreenWidth();
	static i32 ScreenHeight();
	static OPfloat AspectRatio();

	static void Shutdown();
private:
	static Renderer* m_renderer;
	static i32 _width;
	static i32 _height;
};
