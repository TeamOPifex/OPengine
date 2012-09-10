#include "GLTypes.h"

using namespace OPifex;

GLenum OPifex::ShaderTypes::gl_shader_type(OPifex::ShaderTypes::ShaderType shaderType){
	switch(shaderType){
		case ShaderTypes::Vertex: return GL_VERTEX_SHADER_ARB;
		case ShaderTypes::Fragment: return GL_FRAGMENT_SHADER_ARB;
		default: break;
	}

	return GL_VERTEX_SHADER_ARB;
}

OPifex::ShaderTypes::ShaderType OPifex::ShaderTypes::to_shader_type(GLenum shaderType){
	switch(shaderType){
		case GL_VERTEX_SHADER_ARB: return ShaderTypes::Vertex;
		case GL_FRAGMENT_SHADER_ARB: return ShaderTypes::Fragment;
		default:break;
	}

	return ShaderTypes::Vertex;
}