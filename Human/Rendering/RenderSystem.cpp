#include "RenderSystem.h"
#include "GL/GLRenderer.h"
#include "GL/ES/GLESRenderer.h"

using namespace OPifex;

Renderer* RenderSystem::m_renderer = 0;

int RenderSystem::Initialize(RendererType renderer){
	switch(renderer){
	case OpenGL_3_3: m_renderer = new GLRenderer();
	case OpenGL_ES_2_0: m_renderer = new GLESRenderer();
	default: m_renderer = new GLRenderer();
	}
	
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

void RenderSystem::SetBuffer(ui32 buffer){
	m_renderer->set_buffer(buffer);
}

void RenderSystem::RenderTriangles(ui32 offset, ui32 count){
	m_renderer->render_triangles(offset, count);
}

void RenderSystem::Present(){
	m_renderer->swap_buffer();
}

bool RenderSystem::escape(){
	return true;// glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetWindowParam( GLFW_OPENED );
}

void RenderSystem::Shutdown(){
	m_renderer->shutdown();
}