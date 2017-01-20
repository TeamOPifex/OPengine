#include "./Human/include/Platform/opengl/OPeffectAPIGL.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Platform/opengl/OPshaderAPIGL.h"
#include "./Human/include/Rendering/OPshaderUniform.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Core/include/OPmemory.h"

bool OPeffectGLAddUniform(OPeffect* effect, const OPchar* name);

OPeffect* OPeffectAPIGLInit(OPeffect* effect, OPshader* vert, OPshader* frag, OPvertexLayout* vertexLayout) {
	OPeffectGL* effectGL = (OPeffectGL*)OPalloc(sizeof(OPeffectGL));

	effect->uniforms.Init(128);

	effect->internalPtr = effectGL;
	effect->vertexShader = vert;
	effect->fragmentShader = frag;

	OPshaderGL* vertGL = (OPshaderGL*)vert->internalPtr;
	OPshaderGL* fragGL = (OPshaderGL*)frag->internalPtr;

	OPGLFN(effectGL->Handle = glCreateProgram());

	OPGLFN(glAttachShader(effectGL->Handle, vertGL->Handle));
	OPGLFN(glAttachShader(effectGL->Handle, fragGL->Handle));

	if (vertexLayout != NULL) {
		for (ui32 i = 0; i < vertexLayout->count; i++) {
			glBindAttribLocation(effectGL->Handle, i, vertexLayout->attributes[i].Name);
		}
	}

	OPGLFN(glLinkProgram(effectGL->Handle));

	i32 status;
	OPGLFN(glGetProgramiv(effectGL->Handle, GL_LINK_STATUS, &status));

	if (status == GL_FALSE) {
		GLsizei length;
		OPGLFN(glGetProgramInfoLog(effectGL->Handle, OPSCRATCHBUFFER_SIZE, &length, OPSCRATCHBUFFER));
		OPlog("Program Log: %s", OPSCRATCHBUFFER);
		return NULL;
	}
	else {
		GLint i;
		GLint count;
		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 32; // maximum name length
		GLchar name[bufSize]; // variable name in GLSL
		GLsizei length; // name length

		glGetProgramiv(effectGL->Handle, GL_ACTIVE_ATTRIBUTES, &count);
		OPlogChannel((ui32)OPlogLevel::VERBOSE, "SHADER", "Active Attributes: %d", count);
		
		for (i = 0; i < count; i++)
		{
			glGetActiveAttrib(effectGL->Handle, (GLuint)i, bufSize, &length, &size, &type, name);
			i32 result = glGetAttribLocation(effectGL->Handle, name);

			OPlogChannel((ui32)OPlogLevel::VERBOSE, "SHADER", "Attribute #%d Type: %u Name: %s, Loc: %d", i, type, name, result);
		}

		glGetProgramiv(effectGL->Handle, GL_ACTIVE_UNIFORMS, &count);
		OPlogChannel((ui32)OPlogLevel::VERBOSE, "SHADER", "Active Uniforms: %d", count);

		for (i = 0; i < count; i++)
		{
			glGetActiveUniform(effectGL->Handle, (GLuint)i, bufSize, &length, &size, &type, name);
			if (OPstringEquals("uBones[0]", name)) {
				//ui32 loc = glGetUniformLocation(effectGL->Handle, "uBones");
				//OPlogInfo("Bone Location: %d", loc);
				OPeffectGLAddUniform(effect, "uBones");
			}
			else {
				OPeffectGLAddUniform(effect, name);
			}
			OPlogChannel((ui32)OPlogLevel::VERBOSE, "SHADER", "Uniform #%d Type: %u Name: %s", i, type, name);
		}

	}

	return effect;
}

OPeffect* OPeffectGLCreate(OPshader* vert, OPshader* frag, OPvertexLayout* vertexLayot) {
	OPeffect* effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	effect->Init(vert, frag, vertexLayot);
	return effect;
}

void OPeffectGLSetVertexLayout(OPeffect* effect, OPvertexLayout* vertexLayout) {
	OPeffectGL* effectGL = (OPeffectGL*)effect->internalPtr;
	//OPvertexLayoutGL* vertexLayoutGL = (OPvertexLayoutGL*)vertexLayout->internalPtr;

	ui32 i = 0;
	for (; i < vertexLayout->count; i++)
	{
        glBindAttribLocation(effectGL->Handle, i, vertexLayout->attributes[i].Name);		
        vertexLayout->attributes[i].Location = glGetAttribLocation( effectGL->Handle, vertexLayout->attributes[i].Name );
    }
}

bool OPeffectGLAddUniform(OPeffect* effect, const OPchar* name) {
	OPshaderUniform* shaderUniform = OPRENDERER_ACTIVE->ShaderUniform.Create(effect, name);
	if (!shaderUniform->Found) {
		return false;
	}
	effect->uniforms.Put(name, shaderUniform);
	return true;
}

void OPeffectGLBind(OPeffect* effect) {
	OPeffectGL* effectGL = (OPeffectGL*)effect->internalPtr;
	//for (OPuint i  = 0; i < 8; i++) {
	//	OPGLFN(glDisableVertexAttribArray(i));
	//}
	OPGLFN(glUseProgram(effectGL->Handle));
	OPRENDERER_ACTIVE->OPEFFECT_ACTIVE = effect;
}

void OPeffectGLUnbind(OPeffect* effect) {
	OPGLFN(glUseProgram(0));
	OPRENDERER_ACTIVE->OPEFFECT_ACTIVE = NULL;
}

void OPeffectGLDestroy(OPeffect* effect) {
	OPeffectGL* effectGL = (OPeffectGL*)effect->internalPtr;

	OPhashMapBucket* bucket;
	OPuint i, j, n, m;
	OPhashMapPair *pair;

	n = effect->uniforms.count;
	bucket = effect->uniforms.buckets;
	i = 0;
	while (i < n) {
		m = bucket->count;
		pair = bucket->pairs;
		j = 0;
		while (j < m) {
			// mark asset for removal
			OPshaderUniform* shaderUniform = (OPshaderUniform*)pair->value;
			shaderUniform->Destroy();
			OPfree(shaderUniform);
			pair++;
			j++;
		}
		bucket++;
		i++;
	}

	effect->uniforms.Destroy();

	OPeffectGLUnbind(effect);
	OPGLFN(glDeleteProgram(effectGL->Handle));
	OPfree(effectGL);
	effect->internalPtr = NULL;
}

void OPeffectAPIGLInit(OPeffectAPI* effect) {
	effect->_Init = OPeffectAPIGLInit;
	effect->_Create = OPeffectGLCreate;
	effect->AddUniform = OPeffectGLAddUniform;
    effect->SetVertexLayout = OPeffectGLSetVertexLayout;
	effect->Bind = OPeffectGLBind;
	effect->Unbind = OPeffectGLUnbind;
	effect->Destroy = OPeffectGLDestroy;
}
