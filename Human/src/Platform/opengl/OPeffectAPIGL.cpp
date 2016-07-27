#include "./Human/include/Platform/opengl/OPeffectAPIGL.h"
#include "./Human/include/Platform/opengl/OPshaderAPIGL.h"
#include "./Human/include/Rendering/OPshaderUniform.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Core/include/OPmemory.h"

OPeffect* OPeffectAPIGLInit(OPeffect* effect, OPshader* vert, OPshader* frag) {
	OPeffectGL* effectGL = (OPeffectGL*)OPalloc(sizeof(OPeffectGL));
	effect->internalPtr = effectGL;
	effect->vertexShader = vert;
	effect->fragmentShader = frag;

	OPshaderGL* vertGL = (OPshaderGL*)vert->internalPtr;
	OPshaderGL* fragGL = (OPshaderGL*)frag->internalPtr;

	OPGLFN(effectGL->Handle = glCreateProgram());


	OPGLFN(glAttachShader(effectGL->Handle, vertGL->Handle));
	OPGLFN(glAttachShader(effectGL->Handle, fragGL->Handle));

	OPGLFN(glLinkProgram(effectGL->Handle));

	i32 status;
	OPGLFN(glGetProgramiv(effectGL->Handle, GL_LINK_STATUS, &status));

	if (status == GL_FALSE) {
		GLsizei length;
		OPGLFN(glGetProgramInfoLog(effectGL->Handle, OPSCRATCHBUFFER_SIZE, &length, OPSCRATCHBUFFER));
		OPlog("Program Log: %s", OPSCRATCHBUFFER);
		return NULL;
	}

	return effect;
}

OPeffect* OPeffectGLCreate(OPshader* vert, OPshader* frag) {
	OPeffect* effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	return OPeffectAPIGLInit(effect, vert, frag);
}

bool OPeffectGLAddUniform(OPeffect* effect, const OPchar* name) {
	OPshaderUniform* shaderUniform = OPRENDERER_ACTIVE->ShaderUniform.Create(effect, name);
	if (!shaderUniform->Found) {
		return false;
	}
	OPhashMapPut(&effect->uniforms, name, shaderUniform);
	return true;
}

void OPeffectGLBind(OPeffect* effect) {
	OPeffectGL* effectGL = (OPeffectGL*)effect->internalPtr;
	for (OPuint i  = 0; i < 8; i++) {
		OPGLFN(glDisableVertexAttribArray(i));
	}
	OPGLFN(glUseProgram(effectGL->Handle));
}

void OPeffectGLUnbind(OPeffect* effect) {
	OPGLFN(glUseProgram(0));
}

void OPeffectGLDestroy(OPeffect* effect) {
	OPeffectGL* effectGL = (OPeffectGL*)effect->internalPtr;
	OPGLFN(glDeleteProgram(effectGL->Handle));
}

void OPeffectAPIGLInit(OPeffectAPI* effect) {
	effect->Init = OPeffectAPIGLInit;
	effect->Create = OPeffectGLCreate;
	effect->AddUniform = OPeffectGLAddUniform;
	effect->Bind = OPeffectGLBind;
	effect->Unbind = OPeffectGLUnbind;
	effect->Destroy = OPeffectGLDestroy;
}