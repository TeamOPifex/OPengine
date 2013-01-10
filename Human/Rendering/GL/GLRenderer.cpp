#include "GLRenderer.h"
#ifdef OPIFEX_OPENGL_ES_2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

using namespace glm;
#endif


GLRenderer::GLRenderer(){
}

int GLRenderer::initialize(){
#ifdef OPIFEX_OPENGL_ES_2
	// Android doesn't need to create a window
	return 0;
#else
	// Most of the below will be moved to a Windowing System
	if( !glfwInit() ) return -1;
	
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
	// Open a window and create its OpenGL context
	if( !glfwOpenWindow( 1280, 720, 0,0,0,0, 32,0, GLFW_WINDOW ) )
	{
		glfwTerminate();
		return -1;
	}

	if (glewInit() != GLEW_OK) return -1;	

	glfwSetWindowTitle( "OPifex Engine" );
	glfwEnable( GLFW_STICKY_KEYS );	

	return 0;
#endif
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
#ifdef OPIFEX_OPENGL_ES_2
#else
	glfwSwapBuffers();	
#endif
}

void GLRenderer::shutdown(){	
#ifdef OPIFEX_OPENGL_ES_2
#else
	glfwTerminate();
#endif
}