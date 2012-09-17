#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "GLESRenderer.h"


GLESRenderer::GLESRenderer(){
}

int GLESRenderer::initialize(){
	
	return 0;
}

void GLESRenderer::clear_color(f32 r, f32 g, f32 b){	
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(r, g, b, 1.0f);
}

void GLESRenderer::use_material(MaterialPtr material){
	//glUseProgram(material->handle());
}

void GLESRenderer::set_viewport(ui32 x, ui32 y, ui32 width, ui32 height){
	glViewport(x, y, width, height);
}

void GLESRenderer::set_buffer(ui32 buffer){
	//glBindBuffer(GL_ARRAY_BUFFER, buffer);
}

void GLESRenderer::render_triangles(ui32 offset, ui32 count){
	glDrawArrays(GL_TRIANGLES, offset, count);
}

void GLESRenderer::swap_buffer(){
	//glfwSwapBuffers();	
}

void GLESRenderer::shutdown(){	
	//glfwTerminate();
}