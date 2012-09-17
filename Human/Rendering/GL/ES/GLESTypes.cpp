#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "GLESTypes.h"

GLenum ShaderTypes::gl_shader_type(ShaderTypes::ShaderType shaderType){
	switch(shaderType){
		case ShaderTypes::Vertex: return GL_VERTEX_SHADER;
		case ShaderTypes::Fragment: return GL_FRAGMENT_SHADER;
		default: break;
	}

	return GL_VERTEX_SHADER;
}

ShaderTypes::ShaderType ShaderTypes::to_shader_type(GLenum shaderType){
	switch(shaderType){
		case GL_VERTEX_SHADER: return ShaderTypes::Vertex;
		case GL_FRAGMENT_SHADER: return ShaderTypes::Fragment;
		default:break;
	}

	return ShaderTypes::Vertex;
}
