#include "GLTypes.h"
#ifdef OPIFEX_OPENGL_ES_2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

ui32 gl_shader_type(ShaderType shaderType){
#ifdef OPIFEX_OPENGL_ES_2
	switch(shaderType){
		case Vertex: return GL_VERTEX_SHADER;
		case Fragment: return GL_FRAGMENT_SHADER;
		default: break;
	}

	return GL_VERTEX_SHADER;
#else
	switch(shaderType){
		case Vertex: return GL_VERTEX_SHADER_ARB;
		case Fragment: return GL_FRAGMENT_SHADER_ARB;
		default: break;
	}

	return GL_VERTEX_SHADER_ARB;
#endif
}

ShaderType to_shader_type(ui32 shaderType){
#ifdef OPIFEX_OPENGL_ES_2
	switch(shaderType){
		case GL_VERTEX_SHADER: return Vertex;
		case GL_FRAGMENT_SHADER: return Fragment;
		default:break;
	}

	return Vertex;
#else
	switch(shaderType){
		case GL_VERTEX_SHADER_ARB: return Vertex;
		case GL_FRAGMENT_SHADER_ARB: return Fragment;
		default:break;
	}

	return Vertex;
#endif
}