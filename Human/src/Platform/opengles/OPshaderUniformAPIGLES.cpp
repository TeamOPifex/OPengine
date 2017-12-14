
#include "./Human/include/Platform/opengles/OPshaderUniformAPIGLES.h"

#ifdef OPIFEX_OPENGL_ES_2

#include "./Human/include/Platform/opengles/OPcommonGLES.h"
#include "./Human/include/Platform/opengles/OPeffectAPIGLES.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPshaderUniform.h"
#include "./Core/include/Assert.h"

OPshaderUniform* OPshaderUniformGLESInit(OPshaderUniform* shaderUniform, OPeffect* effect, const OPchar* name) {
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)OPalloc(sizeof(OPshaderUniformGLES));;
	shaderUniform->internalPtr = shaderUniformGL;

	OPeffectGLES* effectGL = (OPeffectGLES*)effect->internalPtr;


	GLint result = glGetUniformLocation(effectGL->Handle, name);
	if (result > -1) {
		shaderUniformGL->Handle = result;
	}

	shaderUniform->Found = result > -1;

	return shaderUniform;
}

OPshaderUniform* OPshaderUniformGLESCreate(OPeffect* effect, const OPchar* name) {
	OPshaderUniform* shaderUniform = (OPshaderUniform*)OPalloc(sizeof(OPshaderUniform));
	return OPshaderUniformGLESInit(shaderUniform, effect, name);
}

void OPshaderUniformGLESDestroy(OPshaderUniform* shaderUniform) {
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	OPfree(shaderUniformGL);
	shaderUniform->internalPtr = NULL;
}

void OPshaderUniformSetBoolGLES(OPshaderUniform* shaderUniform, bool val) {
#ifdef _DEBUG
	if (shaderUniform == NULL) return;
#endif
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	glUniform1i(shaderUniformGL->Handle, val);
}
void OPshaderUniformSetFGLES(OPshaderUniform* shaderUniform, f32 val) {
#ifdef _DEBUG
	if (shaderUniform == NULL) return;
#endif
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	glUniform1f(shaderUniformGL->Handle, val);
}
void OPshaderUniformSetFvGLES(OPshaderUniform* shaderUniform, ui32 count, f32* val) {
#ifdef _DEBUG
	if (shaderUniform == NULL) return;
#endif
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	glUniform1fv(shaderUniformGL->Handle, count, val);
}
void OPshaderUniformSetIGLES(OPshaderUniform* shaderUniform, i32 val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	glUniform1i(shaderUniformGL->Handle, val);
}
void OPshaderUniformSetIvGLES(OPshaderUniform* shaderUniform, ui32 count, i32* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	glUniform1iv(shaderUniformGL->Handle, count, val);
}
void OPshaderUniformSetVec2GLES(OPshaderUniform* shaderUniform, OPvec2* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	glUniform2fv(shaderUniformGL->Handle, 1, (OPfloat*)val);
}
void OPshaderUniformSetVec2vGLES(OPshaderUniform* shaderUniform, ui32 count, OPvec2* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	glUniform2fv(shaderUniformGL->Handle, count, (OPfloat*)val);
}
void OPshaderUniformSetVec3GLES(OPshaderUniform* shaderUniform, OPvec3* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	glUniform3fv(shaderUniformGL->Handle, 1, (OPfloat*)val);
}
void OPshaderUniformSetVec3vGLES(OPshaderUniform* shaderUniform, ui32 count, OPvec3* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	glUniform3fv(shaderUniformGL->Handle, count, (OPfloat*)val);
}
void OPshaderUniformSetVec4GLES(OPshaderUniform* shaderUniform, OPvec4* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	glUniform4fv(shaderUniformGL->Handle, 1, (OPfloat*)val);
}
void OPshaderUniformSetVec4vGLES(OPshaderUniform* shaderUniform, ui32 count, OPvec4* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	glUniform4fv(shaderUniformGL->Handle, count, (OPfloat*)val);
}
void OPshaderUniformSetMat3GLES(OPshaderUniform* shaderUniform, OPmat3* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	glUniformMatrix3fv(shaderUniformGL->Handle, 1, GL_FALSE, (f32*)val);
}
void OPshaderUniformSetMat3vGLES(OPshaderUniform* shaderUniform, ui32 count, OPmat3* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	glUniformMatrix3fv(shaderUniformGL->Handle, count, GL_FALSE, (f32*)val);
}
void OPshaderUniformSetMat4GLES(OPshaderUniform* shaderUniform, OPmat4* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	glUniformMatrix4fv(shaderUniformGL->Handle, 1, GL_FALSE, (f32*)val);
}
void OPshaderUniformSetMat4vGLES(OPshaderUniform* shaderUniform, ui32 count, OPmat4* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	glUniformMatrix4fv(shaderUniformGL->Handle, count, GL_FALSE, (f32*)val);
}
void OPshaderUniformSetTextureGLES(OPshaderUniform* shaderUniform, OPtexture* val, ui32 slot) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
	#else
		if (shaderUniform == NULL) {
			OPlogErr("Shader Uniform was NULL");
			return;
		}
    #endif
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	val->Bind(slot);
	glUniform1i(shaderUniformGL->Handle, slot);
}
void OPshaderUniformSetTexturevGLES(OPshaderUniform* shaderUniform, ui32 count, OPtexture* val, ui32 slot) {
	ASSERT(true, "NOT SUPPORTED YET");
}
void OPshaderUniformSetTextureCubeGLES(OPshaderUniform* shaderUniform, OPtextureCube* val, ui32 slot) {
	#ifdef _DEBUG
		if (shaderUniform == NULL) return;
	#endif
	OPshaderUniformGLES* shaderUniformGL = (OPshaderUniformGLES*)shaderUniform->internalPtr;
	val->Bind(slot);
	glUniform1i(shaderUniformGL->Handle, slot);

}
void OPshaderUniformSetTextureCubevGLES(OPshaderUniform* shaderUniform, ui32 count, OPtextureCube* val, ui32 slot) {
	ASSERT(true, "NOT SUPPORTED YET");
}

void OPshaderUniformAPIGLESInit(OPshaderUniformAPI* shaderUniform) {
	shaderUniform->Create = OPshaderUniformGLESCreate;
	shaderUniform->Init = OPshaderUniformGLESInit;
	shaderUniform->Destroy = OPshaderUniformGLESDestroy;

	shaderUniform->SetBool = OPshaderUniformSetBoolGLES;
	shaderUniform->SetF = OPshaderUniformSetFGLES;
	shaderUniform->SetFv = OPshaderUniformSetFvGLES;
	shaderUniform->SetI = OPshaderUniformSetIGLES;
	shaderUniform->SetIv = OPshaderUniformSetIvGLES;
	shaderUniform->SetVec2 = OPshaderUniformSetVec2GLES;
	shaderUniform->SetVec2v = OPshaderUniformSetVec2vGLES;
	shaderUniform->SetVec3 = OPshaderUniformSetVec3GLES;
	shaderUniform->SetVec3v = OPshaderUniformSetVec3vGLES;
	shaderUniform->SetVec4 = OPshaderUniformSetVec4GLES;
	shaderUniform->SetVec4v = OPshaderUniformSetVec4vGLES;
	shaderUniform->SetMat3 = OPshaderUniformSetMat3GLES;
	shaderUniform->SetMat3v = OPshaderUniformSetMat3vGLES;
	shaderUniform->SetMat4 = OPshaderUniformSetMat4GLES;
	shaderUniform->SetMat4v = OPshaderUniformSetMat4vGLES;
	shaderUniform->SetTexture = OPshaderUniformSetTextureGLES;
	shaderUniform->SetTexturev = OPshaderUniformSetTexturevGLES;
	shaderUniform->SetTextureCube = OPshaderUniformSetTextureCubeGLES;
	shaderUniform->SetTextureCubev = OPshaderUniformSetTextureCubevGLES;

}

#endif