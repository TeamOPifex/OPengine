#include "GLTypes.h"

GLenum ShaderTypes::gl_shader_type(ShaderTypes::ShaderType shaderType){
	switch(shaderType){
		case ShaderTypes::Vertex: return GL_VERTEX_SHADER_ARB;
		case ShaderTypes::Fragment: return GL_FRAGMENT_SHADER_ARB;
		default: break;
	}

	return GL_VERTEX_SHADER_ARB;
}

ShaderTypes::ShaderType ShaderTypes::to_shader_type(GLenum shaderType){
	switch(shaderType){
		case GL_VERTEX_SHADER_ARB: return ShaderTypes::Vertex;
		case GL_FRAGMENT_SHADER_ARB: return ShaderTypes::Fragment;
		default:break;
	}

	return ShaderTypes::Vertex;
}