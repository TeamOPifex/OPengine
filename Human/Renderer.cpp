#include "Renderer.h"
#include "GLESRenderer.h"
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

using namespace OPifex;
using namespace glm;

ApiRenderer* Renderer::m_api_renderer = 0;

int Renderer::Initialize(){
	m_api_renderer = new GLESRenderer();
	
	if( !glfwInit() )
	{
		//fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}
		
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
	// Open a window and create its OpenGL context
	if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
	{
		//fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}

	if (glewInit() != GLEW_OK) {
		//fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetWindowTitle( "OPifex Engine" );
	glfwEnable( GLFW_STICKY_KEYS );	

	return 0;
}

void Renderer::clear_color(f32 r, f32 g, f32 b){
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(r, g, b, 1.0f);
	glfwSwapBuffers();
}

bool Renderer::escape(){
	return glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
			glfwGetWindowParam( GLFW_OPENED );
}

void Renderer::shutdown(){	
	glfwTerminate();
}