
#include "./Human/include/Platform/opengl/OPshaderAPIGL.h"

#ifdef OPIFEX_OPENGL

#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPshader.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"

const OPchar* OPshaderTypeToString(OPshaderType::Enum shaderType) {
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

OPshader* OPshaderGLInit(OPshader* shader, OPshaderType::Enum shaderType, const OPchar* source, OPuint sourceLen) {
	OPshaderGL* shaderGL = (OPshaderGL*)OPalloc(sizeof(OPshaderGL));
	shader->internalPtr = shaderGL;
	shader->shaderType = shaderType;

	// Set the Shader Type
	switch (shaderType) {
		case OPshaderType::VERTEX: {
			OPGLFN(shaderGL->Handle = glCreateShader(GL_VERTEX_SHADER));
			break;
		}
		case OPshaderType::FRAGMENT: {
			OPGLFN(shaderGL->Handle = glCreateShader(GL_FRAGMENT_SHADER));
			break;
		}
	}

	// Add the shader source to the shader
	i32 length = (i32)sourceLen;
	OPGLFN(glShaderSource(shaderGL->Handle, 1, &source, &length));

	// Attempt to compile the shader code
	OPGLFN(glCompileShader(shaderGL->Handle));

	// Make sure that the shader compiled successfully
	GLint compileResult = 0;
	OPGLFN(glGetShaderiv(shaderGL->Handle, GL_COMPILE_STATUS, &compileResult));

	if (!compileResult) {
		i32 length = 0;
		OPlogErr("Failed to compile %s Shader", OPshaderTypeToString(shaderType));
		OPGLFN(glGetShaderInfoLog(shaderGL->Handle, OPSCRATCHBUFFER_SIZE, &length, OPSCRATCHBUFFER));
		OPlogErr("OPshaderGLInit::%s", OPSCRATCHBUFFER);
		OPGLFN(glDeleteShader(shaderGL->Handle));
		shaderGL->Compiled = false;
	}
	else {
		shaderGL->Compiled = true;
	}

	return shader;
}

OPshader* OPshaderGLCreate(OPshaderType::Enum shaderType, const OPchar* source, OPuint sourceLen) {
	OPshader* shader = (OPshader*)OPalloc(sizeof(OPshader));
	return OPshaderGLInit(shader, shaderType, source, sourceLen);
}

void OPshaderGLDestroy(OPshader* shader) {
	OPshaderGL* shaderGL = (OPshaderGL*)shader->internalPtr;
	OPGLFN(glDeleteShader(shaderGL->Handle));
	OPfree(shaderGL);
	shader->internalPtr = NULL;
}

void OPshaderAPIGLInit(OPshaderAPI* shader) {
	shader->_Init = OPshaderGLInit;
	shader->_Create = OPshaderGLCreate;
	shader->Destroy = OPshaderGLDestroy;
}

#endif