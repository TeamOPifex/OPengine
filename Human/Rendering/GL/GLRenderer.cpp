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

#include "./Core/include/Log.h"

GLRenderer::GLRenderer(){
}

int GLRenderer::initialize(ui32 width, ui32 height){

#ifdef OPIFEX_OPENGL_ES_2
	// Android doesn't need to create a window
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	//glCullFace(GL_FRONT);
	//glEnable(GL_CULL_FACE);
	return 0;
#else
	// Most of the below will be moved to a Windowing System
	if( !glfwInit() ) return -1;
	
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
	// Open a window and create its OpenGL context
	if( !glfwOpenWindow( width, height, 0,0,0,0, 32,0, GLFW_WINDOW ) )
	{		
		OPLog("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" );
		glfwTerminate();
		return -1;
	}

	//glewExperimental = true;
	if (glewInit() != GLEW_OK) return -1;	

	glfwSetWindowTitle( "OPifex Engine" );
	glfwEnable( GLFW_STICKY_KEYS );	

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS); 
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	// TODO: Determine how to optimize with this
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
#endif
}

void GLRenderer::clear_color(f32 r, f32 g, f32 b){	
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::use_material(MaterialPtr material){
	glUseProgram(material->Handle());
}

void GLRenderer::set_viewport(ui32 x, ui32 y, ui32 width, ui32 height){
	glViewport(x, y, width, height);
}

void GLRenderer::set_buffer(int bufferType, ui32 buffer){
	if(bufferType == 1)
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
	else if(bufferType == 2)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
}

void GLRenderer::render_triangles(ui32 offset, ui32 count){
	glDrawArrays(GL_TRIANGLES, offset, count);
}

void GLRenderer::render_triangles(ui32 numIndices){
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
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

void GLRenderer::depth_test(bool state){
	if(state)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}