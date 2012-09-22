#include "GLTypes.h"

GLenum gl_shader_type(ShaderType shaderType){
	switch(shaderType){
		case Vertex: return GL_VERTEX_SHADER_ARB;
		case Fragment: return GL_FRAGMENT_SHADER_ARB;
		default: break;
	}

	return GL_VERTEX_SHADER_ARB;
}

ShaderType to_shader_type(GLenum shaderType){
	switch(shaderType){
		case GL_VERTEX_SHADER_ARB: return Vertex;
		case GL_FRAGMENT_SHADER_ARB: return Fragment;
		default:break;
	}

	return Vertex;
}