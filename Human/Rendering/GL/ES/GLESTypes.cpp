#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "GLESTypes.h"

GLenum gl_shader_type(ShaderType shaderType){
	switch(shaderType){
		case Vertex: return GL_VERTEX_SHADER;
		case Fragment: return GL_FRAGMENT_SHADER;
		default: break;
	}

	return GL_VERTEX_SHADER;
}

ShaderType to_shader_type(GLenum shaderType){
	switch(shaderType){
		case GL_VERTEX_SHADER: return Vertex;
		case GL_FRAGMENT_SHADER: return Fragment;
		default:break;
	}

	return Vertex;
}
