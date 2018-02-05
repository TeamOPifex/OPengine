
#include "./Human/include/Platform/opengl/OPshaderUniformAPIGL.h"

#ifdef OPIFEX_OPENGL

#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Platform/opengl/OPeffectAPIGL.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPshaderUniform.h"
#include "./Core/include/Assert.h"

OPshaderUniform* OPshaderUniformGLInit(OPshaderUniform* shaderUniform, OPeffect* effect, ui32 ind) {
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)OPalloc(sizeof(OPshaderUniformGL));;
	shaderUniform->internalPtr = shaderUniformGL;

	OPeffectGL* effectGL = (OPeffectGL*)effect->internalPtr;


	GLint i;
	GLint size; // size of the variable
	GLenum type; // type of the variable (float, vec3 or mat4, etc)

	const GLsizei bufSize = 64; // maximum name length
	GLchar name[bufSize]; // variable name in GLSL
	GLsizei length; // name length
	glGetActiveUniform(effectGL->Handle, (GLuint)ind, bufSize, &length, &size, &type, name);

	OPlogChannel((ui32)OPlogLevel::VERBOSE, "SHADER", "   Uniform #%d Type: %u Name: %s", ind, type, name);



	GLint result = glGetUniformLocation(effectGL->Handle, name);
	if (result > -1) {
		shaderUniformGL->Handle = result;
	}

	shaderUniform->Found = result > -1;
	shaderUniform->name = OPstringCopy(name);
	shaderUniform->count = size;
	shaderUniform->type = UniformTypeToOPshaderUniformType(type);

	return shaderUniform;
}

OPshaderUniform* OPshaderUniformGLCreate(OPeffect* effect, ui32 loc) {
	OPshaderUniform* shaderUniform = (OPshaderUniform*)OPalloc(sizeof(OPshaderUniform));
	return OPshaderUniformGLInit(shaderUniform, effect, loc);
}

void OPshaderUniformGLDestroy(OPshaderUniform* shaderUniform) {
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	OPfree(shaderUniformGL);
	shaderUniform->internalPtr = NULL;
}

void OPshaderUniformSetBoolGL(OPshaderUniform* shaderUniform, bool val) {
#ifdef _DEBUG
	if (shaderUniform == NULL) return;
#endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform1i(shaderUniformGL->Handle, val);
}
void OPshaderUniformSetFGL(OPshaderUniform* shaderUniform, f32 val) {
#ifdef _DEBUG
	if (shaderUniform == NULL) return;
#endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform1f(shaderUniformGL->Handle, val);
}
void OPshaderUniformSetFvGL(OPshaderUniform* shaderUniform, ui32 count, f32* val) {
#ifdef _DEBUG
	if (shaderUniform == NULL) return;
#endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform1fv(shaderUniformGL->Handle, count, val);
}
void OPshaderUniformSetIGL(OPshaderUniform* shaderUniform, i32 val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform1i(shaderUniformGL->Handle, val);
}
void OPshaderUniformSetIvGL(OPshaderUniform* shaderUniform, ui32 count, i32* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform1iv(shaderUniformGL->Handle, count, val);
}
void OPshaderUniformSetVec2GL(OPshaderUniform* shaderUniform, OPvec2* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform2fv(shaderUniformGL->Handle, 1, (OPfloat*)val);
}
void OPshaderUniformSetVec2vGL(OPshaderUniform* shaderUniform, ui32 count, OPvec2* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform2fv(shaderUniformGL->Handle, count, (OPfloat*)val);
}
void OPshaderUniformSetVec3GL(OPshaderUniform* shaderUniform, OPvec3* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform3fv(shaderUniformGL->Handle, 1, (OPfloat*)val);
}
void OPshaderUniformSetVec3vGL(OPshaderUniform* shaderUniform, ui32 count, OPvec3* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform3fv(shaderUniformGL->Handle, count, (OPfloat*)val);
}
void OPshaderUniformSetVec4GL(OPshaderUniform* shaderUniform, OPvec4* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform4fv(shaderUniformGL->Handle, 1, (OPfloat*)val);
}
void OPshaderUniformSetVec4vGL(OPshaderUniform* shaderUniform, ui32 count, OPvec4* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform4fv(shaderUniformGL->Handle, count, (OPfloat*)val);
}
void OPshaderUniformSetMat3GL(OPshaderUniform* shaderUniform, OPmat3* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniformMatrix3fv(shaderUniformGL->Handle, 1, GL_FALSE, (f32*)val);
}
void OPshaderUniformSetMat3vGL(OPshaderUniform* shaderUniform, ui32 count, OPmat3* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniformMatrix3fv(shaderUniformGL->Handle, count, GL_FALSE, (f32*)val);
}
void OPshaderUniformSetMat4GL(OPshaderUniform* shaderUniform, OPmat4* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniformMatrix4fv(shaderUniformGL->Handle, 1, GL_FALSE, (f32*)val);
}
void OPshaderUniformSetMat4vGL(OPshaderUniform* shaderUniform, ui32 count, OPmat4* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniformMatrix4fv(shaderUniformGL->Handle, count, GL_FALSE, (f32*)val);
}
void OPshaderUniformSetTextureGL(OPshaderUniform* shaderUniform, OPtexture* val, ui32 slot) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
	#else
		if (shaderUniform == NULL) {
			OPlogErr("Shader Uniform was NULL");
			return;
		}
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	val->Bind(slot);
	glUniform1i(shaderUniformGL->Handle, slot);
}
void OPshaderUniformSetTexturevGL(OPshaderUniform* shaderUniform, ui32 count, OPtexture* val, ui32 slot) {
	ASSERT(true, "NOT SUPPORTED YET");
}
void OPshaderUniformSetTextureCubeGL(OPshaderUniform* shaderUniform, OPtextureCube* val, ui32 slot) {
	#ifdef _DEBUG
		if (shaderUniform == NULL) return;
	#endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	val->Bind(slot);
	glUniform1i(shaderUniformGL->Handle, slot);

}
void OPshaderUniformSetTextureCubevGL(OPshaderUniform* shaderUniform, ui32 count, OPtextureCube* val, ui32 slot) {
	ASSERT(true, "NOT SUPPORTED YET");
}




void OPshaderUniformSetDataGL(OPshaderUniform* shaderUniform, void* data, ui32 loc) {
#ifdef _DEBUG
	if (shaderUniform == NULL) return;
#endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;

	switch (shaderUniform->type) {
		case OPshaderUniformType::BOOL: {
			glUniform1i(shaderUniformGL->Handle, *(bool*)data);
			break;
		}
		case OPshaderUniformType::FLOAT: {
			glUniform1fv(shaderUniformGL->Handle, shaderUniform->count, (f32*)data);
			break;
		}
		case OPshaderUniformType::INT: {
			glUniform1iv(shaderUniformGL->Handle, shaderUniform->count, (i32*)data);
			break;
		}
		case OPshaderUniformType::VECTOR2: {
			glUniform2fv(shaderUniformGL->Handle, shaderUniform->count, (OPfloat*)data);
			break;
		}
		case OPshaderUniformType::VECTOR3: {
			glUniform3fv(shaderUniformGL->Handle, shaderUniform->count, (OPfloat*)data);
			break;
		}
		case OPshaderUniformType::VECTOR4: {
			glUniform4fv(shaderUniformGL->Handle, shaderUniform->count, (OPfloat*)data);
			break;
		}
		case OPshaderUniformType::MATRIX2: {
			glUniformMatrix2fv(shaderUniformGL->Handle, shaderUniform->count, GL_FALSE, (f32*)data);
			break;
		}
		case OPshaderUniformType::MATRIX3: {
			glUniformMatrix3fv(shaderUniformGL->Handle, shaderUniform->count, GL_FALSE, (f32*)data);
			break;
		}
		case OPshaderUniformType::MATRIX4: {
			glUniformMatrix4fv(shaderUniformGL->Handle, shaderUniform->count, GL_FALSE, (f32*)data);
			break;
		}
		case OPshaderUniformType::TEXTURE: {
			OPtexture* val = (OPtexture*)data;
			val->Bind(loc);
			glUniform1i(shaderUniformGL->Handle, loc);
			break;
		}
		case OPshaderUniformType::TEXTURE_CUBE: {
			OPtextureCube* val = (OPtextureCube*)data;
			val->Bind(loc);
			glUniform1i(shaderUniformGL->Handle, loc);
			break;
		}
		default:
			OPlogErr("No type for %s", shaderUniform->name);
			break;
	}
}

void OPshaderUniformAPIGLInit(OPshaderUniformAPI* shaderUniform) {
	shaderUniform->Create = OPshaderUniformGLCreate;
	shaderUniform->Init = OPshaderUniformGLInit;
	shaderUniform->Destroy = OPshaderUniformGLDestroy;
	
	shaderUniform->SetData = OPshaderUniformSetDataGL;
	shaderUniform->SetBool = OPshaderUniformSetBoolGL;
	shaderUniform->SetF = OPshaderUniformSetFGL;
	shaderUniform->SetFv = OPshaderUniformSetFvGL;
	shaderUniform->SetI = OPshaderUniformSetIGL;
	shaderUniform->SetIv = OPshaderUniformSetIvGL;
	shaderUniform->SetVec2 = OPshaderUniformSetVec2GL;
	shaderUniform->SetVec2v = OPshaderUniformSetVec2vGL;
	shaderUniform->SetVec3 = OPshaderUniformSetVec3GL;
	shaderUniform->SetVec3v = OPshaderUniformSetVec3vGL;
	shaderUniform->SetVec4 = OPshaderUniformSetVec4GL;
	shaderUniform->SetVec4v = OPshaderUniformSetVec4vGL;
	shaderUniform->SetMat3 = OPshaderUniformSetMat3GL;
	shaderUniform->SetMat3v = OPshaderUniformSetMat3vGL;
	shaderUniform->SetMat4 = OPshaderUniformSetMat4GL;
	shaderUniform->SetMat4v = OPshaderUniformSetMat4vGL;
	shaderUniform->SetTexture = OPshaderUniformSetTextureGL;
	shaderUniform->SetTexturev = OPshaderUniformSetTexturevGL;
	shaderUniform->SetTextureCube = OPshaderUniformSetTextureCubeGL;
	shaderUniform->SetTextureCubev = OPshaderUniformSetTextureCubevGL;

}

#endif