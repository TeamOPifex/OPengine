#pragma once

#include "./Human/Resources/Resource.h"
#include "Core/include/Types.h"
#include "ShaderTypes.h"
#include "./Human/Resources/Texture/Texture2D.h"
#include "./Human/Math/Matrix4.h"

#ifdef OPIFEX_OPENGL_ES_2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

class Shader : public Resource {
public:
	Shader(ShaderType shaderType, const char* source);
	static Shader* FromFile(ShaderType shaderType, const char* file);
};

typedef Shader* ShaderPtr;

#include "./Human/Resources/Material/Material.h"
class ShaderParam {	
public:
	ShaderParam() { }
protected:
	ui32 _handle;
};

class ShaderParamMatrix : ShaderParam {
public:
	ShaderParamMatrix(ui32 program, const char* variable) {
		_handle = glGetAttribLocation(program, variable);
	}
protected:
	void SetMatrix(Matrix4* value) {
		glUniformMatrix4fv(_handle, 1, GL_FALSE, &(*value)[0][0]);
	}
};

class ShaderParamTexture2D : ShaderParam {
public:
	ShaderParamTexture2D(ui32 program, const char* variable) { 		
		_handle = glGetAttribLocation(program, variable);
	}
protected:
	void SetTexture(Texture2D* value) {
		
	}
};

class ShaderParamWorld : public ShaderParamMatrix {
public:
	ShaderParamWorld(ui32 program) : ShaderParamMatrix(program, "mWorld") { }
	void SetWorldMatrix(Matrix4* value) {
		ShaderParamMatrix::SetMatrix(value);
	}
};

class ShaderParamView : public ShaderParamMatrix {
public:
	ShaderParamView(ui32 program) : ShaderParamMatrix(program,"mView") { }
	void SetViewMatrix(Matrix4* value) {
		ShaderParamMatrix::SetMatrix(value);
	}
};

class ShaderParamProjection : public ShaderParamMatrix {
public:
	ShaderParamProjection(ui32 program) : ShaderParamMatrix(program,"mProjection") { }
	void SetProjectionMatrix(Matrix4* value) {
		ShaderParamMatrix::SetMatrix(value);
	}
};

class ShaderParamColorTexture : public ShaderParamTexture2D {
public:
	ShaderParamColorTexture(ui32 program) : ShaderParamTexture2D(program,"tColor") { }
	void SetColorTexture(Texture2D* texture){
		ShaderParamTexture2D::SetTexture(texture);
	}
};

// Textured Material
// Has World, View, and Projection matrices
// Also has a Texture Sampler
class TexturedMaterial : 
	public ShaderParamWorld, 
	public ShaderParamView, 
	public ShaderParamProjection,
	public ShaderParamColorTexture{
public:
	TexturedMaterial() : 
		ShaderParamWorld(0),
		ShaderParamView(0),
		ShaderParamProjection(0),
		ShaderParamColorTexture(0)
	{

	}
};