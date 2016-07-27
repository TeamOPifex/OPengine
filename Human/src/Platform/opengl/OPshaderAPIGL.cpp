#include "./Human/include/Platform/opengl/OPshaderAPIGL.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPshader.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"

const OPchar* OPshaderTypeToString(OPshaderType shaderType) {
	switch (shaderType) {
		case OPshaderType::VERTEX: {
			return "Vertex";
		}
		case OPshaderType::FRAGMENT: {
			return "Fragment";
		}
	}
	return "";
}

OPshader* OPshaderGLInit(OPshader* shader, OPshaderType shaderType, const OPchar* source, OPuint sourceLen) {
	OPshaderGL* shaderGL = (OPshaderGL*)OPalloc(sizeof(OPshaderGL));
	shader->internalPtr = shaderGL;

	// Set the Shader Type
	switch (shaderType) {
		case OPshaderType::VERTEX: {
			OPGLFN(shaderGL->Handle = glCreateShader(GL_VERTEX_SHADER));
		}
		case OPshaderType::FRAGMENT: {
			OPGLFN(shaderGL->Handle = glCreateShader(GL_FRAGMENT_SHADER));
		}
	}

	// Add the shader source to the shader
	GLint length = sourceLen;
	OPGLFN(glShaderSource(shaderGL->Handle, 1, &source, &length));

	// Attempt to compile the shader code
	OPGLFN(glCompileShader(shaderGL->Handle));

	// Make sure that the shader compiled successfully
	GLint compileResult = 0;
	OPGLFN(glGetShaderiv(shaderGL->Handle, GL_COMPILE_STATUS, &compileResult));

	if (!compileResult) {
		char msg[4096];
		i32 length = 0;
		OPGLFN(glGetShaderInfoLog(shaderGL->Handle, 4096, &length, msg));
		OPlogErr("OPshaderGLInit::Failed to compile %s Shader::%s", OPshaderTypeToString(shaderType), msg);
		OPGLFN(glDeleteShader(shaderGL->Handle));
		shaderGL->Compiled = false;
	}
	else {
		shaderGL->Compiled = true;
	}

	return shader;
}

OPshader* OPshaderGLCreate(OPshaderType shaderType, const OPchar* source, OPuint sourceLen) {
	OPshader* shader = (OPshader*)OPalloc(sizeof(OPshader));
	return OPshaderGLInit(shader, shaderType, source, sourceLen);
}

void OPshaderGLDestroy(OPshader* shader) {
	OPshaderGL* shaderGL = (OPshaderGL*)shader->internalPtr;
	OPGLFN(glDeleteShader(shaderGL->Handle));
}

void OPshaderAPIGLInit(OPshaderAPI* shader) {
	shader->_Init = OPshaderGLInit;
	shader->_Create = OPshaderGLCreate;
	shader->Destroy = OPshaderGLDestroy;
}