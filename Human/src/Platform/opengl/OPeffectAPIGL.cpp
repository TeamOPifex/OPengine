#include "./Human/include/Platform/opengl/OPeffectAPIGL.h"
#include "./Human/include/Platform/opengl/OPshaderAPIGL.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Core/include/OPmemory.h"

OPeffect* OPeffectAPIGLInit(OPeffect* effect, OPshader* vert, OPshader* frag) {
	OPeffectGL* effectGL = (OPeffectGL*)OPalloc(sizeof(OPeffectGL));
	effect->internalPtr = effectGL;

	OPGLFN(effectGL->Handle = glCreateProgram());

	OPshaderGL* vertGL = (OPshaderGL*)vert->internalPtr;
	OPshaderGL* fragGL = (OPshaderGL*)frag->internalPtr;

	OPGLFN(glAttachShader(effectGL->Handle, vertGL->Handle));
	OPGLFN(glAttachShader(effectGL->Handle, fragGL->Handle));

	effect->vertexShader = vert;
	effect->fragmentShader = frag;

	OPGLFN(glLinkProgram(effectGL->Handle));

	i32 status;
	OPGLFN(glGetProgramiv(effectGL->Handle, GL_LINK_STATUS, &status));

	if (status == GL_FALSE) {
		OPchar buffer[2048];
		GLsizei length;
		OPGLFN(glGetProgramInfoLog(effectGL->Handle, 2048, &length, buffer));
		OPlog("Program Log: %s", buffer);
		return NULL;
	}

	return effect;
}

OPeffect* OPeffectGLCreate(OPshader* vert, OPshader* frag) {
	OPeffect* effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	return OPeffectAPIGLInit(effect, vert, frag);
}

void OPeffectGLBind(OPeffect* effect) {
	OPeffectGL* effectGL = (OPeffectGL*)effect->internalPtr;
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
	effect->Bind = OPeffectGLBind;
	effect->Unbind = OPeffectGLUnbind;
	effect->Destroy = OPeffectGLDestroy;
}