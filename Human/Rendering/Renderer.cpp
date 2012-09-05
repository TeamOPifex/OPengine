#include "Renderer.h"
#include "GL/GLESRenderer.h"
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

using namespace OPifex;
using namespace glm;

ApiRenderer* Renderer::m_api_renderer = 0;

int Renderer::Initialize(){
	m_api_renderer = new GLESRenderer();
	return m_api_renderer->initialize();
}

void Renderer::ClearColor(f32 r, f32 g, f32 b){
	m_api_renderer->clear_color(r,g,b);
}

void Renderer::Present(){
	m_api_renderer->swap_buffer();
}

bool Renderer::escape(){
	return glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
			glfwGetWindowParam( GLFW_OPENED );
}

void Renderer::Shutdown(){
	m_api_renderer->shutdown();
}