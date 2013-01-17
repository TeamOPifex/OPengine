#include "RenderSystem.h"
#include "GL/GLRenderer.h"
#include "./Core/include/Log.h"

Renderer* RenderSystem::m_renderer = 0;

int RenderSystem::Initialize(){
	m_renderer = new GLRenderer();
	return m_renderer->initialize();
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
	model->ModelMaterial->SetWorldMatrix(&((*model->WorldMatrix)[0][0]));
	SetBuffer(1, model->ModelMesh->VertexBuffer->handle());
	SetBuffer(2, model->ModelMesh->IndexBuffer->handle());
	RenderTriangles(model->ModelMesh->IndexCount);
}

void RenderSystem::Present(){
	m_renderer->swap_buffer();
}

bool RenderSystem::escape(){
	return true;
}

void RenderSystem::Shutdown(){
	m_renderer->shutdown();
}