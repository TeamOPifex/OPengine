#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include "GLRenderer.h"

using namespace OPifex;
using namespace glm;

GLRenderer::GLRenderer(){
}

int GLRenderer::initialize(){

	// Most of the below will be moved to a Windowing System

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

void GLRenderer::clear_color(f32 r, f32 g, f32 b){	
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLRenderer::use_material(MaterialPtr material){
	glUseProgram(material->handle());
}

void GLRenderer::set_viewport(ui32 x, ui32 y, ui32 width, ui32 height){
	glViewport(x, y, width, height);
}

void GLRenderer::set_buffer(ui32 buffer){
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
}

void GLRenderer::render_triangles(ui32 offset, ui32 count){
	glDrawArrays(GL_TRIANGLES, offset, count);
}

void GLRenderer::swap_buffer(){
	glfwSwapBuffers();	
}

void GLRenderer::shutdown(){	
	glfwTerminate();
}