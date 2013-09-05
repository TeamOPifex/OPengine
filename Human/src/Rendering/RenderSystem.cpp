#include "./Human/include/Rendering/RenderSystem.h"
#include "./Human/include/Rendering/GL/GLRenderer.h"
#include "./Core/include/Log.h"

Renderer* RenderSystem::m_renderer = 0;
i32 RenderSystem::_width = 0;
i32 RenderSystem::_height = 0;

int RenderSystem::Initialize(ui32 width, ui32 height){
	m_renderer = new GLRenderer();
	_width = width;
	_height = height;
	return m_renderer->initialize(width, height);
}

void RenderSystem::ClearColor(f32 r, f32 g, f32 b){
	m_renderer->clear_color(r,g,b);
}

void RenderSystem::UseMaterial(MaterialPtr material){
	m_renderer->use_material(material);
}

void RenderSystem::SetViewport(ui32 x, ui32 y, ui32 width, ui32 height){
	m_renderer->set_viewport(x, y, width, height);
}

void RenderSystem::SetBuffer(int bufferType, ui32 buffer){
	m_renderer->set_buffer(bufferType, buffer);
}

void RenderSystem::RenderTriangles(ui32 offset, ui32 count){
	m_renderer->render_triangles(offset, count);
}

void RenderSystem::RenderTriangles(ui32 numIndices){
	m_renderer->render_triangles(numIndices);
}

void RenderSystem::RenderModel(Model* model){
	UseMaterial(model->ModelMaterial);
	model->ModelMaterial->EnableAttributes();
	model->SetMeshData();
	RenderTriangles(model->ModelMesh->IndexCount);
	model->ModelMaterial->DisableAttributes();
}

void RenderSystem::RenderModel(PackedModel* model){
	UseMaterial(model->ModelMaterial);
	model->ModelMaterial->EnableAttributes();
	model->SetMeshData();
	RenderTriangles(model->ModelMesh->IndexCount);
	model->ModelMaterial->DisableAttributes();
}

void RenderSystem::Present(){
	m_renderer->swap_buffer();
}

void RenderSystem::Shutdown(){
	m_renderer->shutdown();
}

void RenderSystem::DepthTest(bool state){
	m_renderer->depth_test(state);
}

i32 RenderSystem::ScreenWidth(){
	return _width;
}

i32 RenderSystem::ScreenHeight(){
	return _height;
}

OPfloat RenderSystem::AspectRatio(){
	if(_width <= 0) return 0;
	return _height / _width;
}

