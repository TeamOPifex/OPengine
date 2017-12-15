
#include "./Human/include/Platform/opengles/OPshaderAPIGLES.h"

#ifdef OPIFEX_OPENGL_ES_2

#include "./Human/include/Platform/opengles/OPcommonGLES.h"
#include "./Human/include/Rendering/OPshader.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"

#include <GLES3/gl3.h>

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

OPshader* OPshaderGLESInit(OPshader* shader, OPshaderType::Enum shaderType, const OPchar* source, OPuint sourceLen) {
	OPshaderGLES* shaderGL = OPNEW(OPshaderGLES);


	// Set the Shader Type
	if(shaderType == OPshaderType::VERTEX) {
		OPlogErr("Create Vertex");
		ui32 handle = glCreateShader(GL_VERTEX_SHADER);
		OPlogErr("Setting Vertex");
		shaderGL->Handle = handle;
	} else {
		OPlogErr("Setting to Fragment");
		shaderGL->Handle = glCreateShader(GL_FRAGMENT_SHADER);
	}

	OPlogErr("Shader Type Created");
	shader->internalPtr = shaderGL;
	shader->shaderType = shaderType;
	
	OPlogErr("Shader Set");

	

	OPlogInfo("Shader (%d) %s", sourceLen, source);

	// Add the shader source to the shader
	i32 length = (i32)sourceLen;
	OPGLFN(glShaderSource(shaderGL->Handle, 1, &source, &length));

	
	OPlogErr("Source Added");

	// Attempt to compile the shader code
	OPGLFN(glCompileShader(shaderGL->Handle));

	OPlogErr("Source Compiled");

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

	OPlogErr("Shader Compiled");

	return shader;
}

OPshader* OPshaderGLESCreate(OPshaderType::Enum shaderType, const OPchar* source, OPuint sourceLen) {
	OPshader* shader = OPNEW(OPshader);
	return OPshaderGLESInit(shader, shaderType, source, sourceLen);
}

void OPshaderGLESDestroy(OPshader* shader) {
	OPshaderGLES* shaderGL = (OPshaderGLES*)shader->internalPtr;
	OPGLFN(glDeleteShader(shaderGL->Handle));
	OPfree(shaderGL);
	shader->internalPtr = NULL;
}

void OPshaderAPIGLESInit(OPshaderAPI* shader) {
	shader->_Init = OPshaderGLESInit;
	shader->_Create = OPshaderGLESCreate;
	shader->Destroy = OPshaderGLESDestroy;
}

#endif