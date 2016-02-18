#ifndef OPENGINE_HUMAN_RENDERER_EFFECT
#define OPENGINE_HUMAN_RENDERER_EFFECT

#include "./Human/include/Utilities/Errors.h"
#include "./Human/include/Rendering/OPshader.h"
#include "./Human/include/Rendering/OPattributes.h"
#include "./Human/include/Rendering/OPvertexLayout.h"
#include "./Human/include/Rendering/OPrenderBuffer.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Rendering/OPcam.h"
#include "OPtextureCube.h"
#include "./Math/include/Vectors.h"
#include "./Math/include/OPmat4.h"
#include "./Data/include/OPhashMap.h"
#include "./Data/include/OPlist.h"
#include "./Data/include/OPfile.h"
#include "./Core/include/OPtypes.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"

#define OP_EFFECT_NAME_LEN 16


//-----------------------------------------------------------------------------
//   _____ _                   _
//  / ____| |                 | |
// | (___ | |_ _ __ _   _  ___| |_ ___
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/

struct OPeffect;
typedef struct OPeffect OPeffect;

//-----------------------------------------------------------------------------
//   _____ _       _           _
//  / ____| |     | |         | |
// | |  __| | ___ | |__   __ _| |___
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/

extern OPeffect* OPEFFECT_ACTIVE;


// effect creation

OPeffect OPeffectCreate(OPshader vert, OPshader frag, OPshaderAttribute* Attributes, OPint AttribCount, const OPchar* Name, ui32 stride);
OPeffect OPeffectCreate(OPshader vert, OPshader frag, OPshaderAttribute* Attributes, OPint AttribCount, const OPchar* Name);
OPeffect OPeffectGen(const OPchar* vert, const OPchar* frag, ui32 attrs, const OPchar* Name, ui32 stride);
OPeffect OPeffectGen(const OPchar* vert, const OPchar* frag, OPvertexLayout* layout);

OPint OPeffectUnload(OPeffect* effect);
void OPeffectUse(OPeffect* effect);
OPint OPeffectBind(OPeffect* effect, ui32 stride);


struct OPeffect {
	OPshader  Vertex;
	OPshader   Fragment;
	ui32      ProgramHandle;
	ui32      Stride;
	OPhashMap*  Parameters;
	OPlist*   Attributes;
	OPchar    Name[OP_EFFECT_NAME_LEN];

	void Init(const OPchar* vert, const OPchar* frag, ui32 attrs, const OPchar* Name, ui32 stride) {
		*this = OPeffectGen(vert, frag, attrs, Name, stride);
	}
	void Init(const OPchar* vert, const OPchar* frag, OPvertexLayout* layout) {
		*this = OPeffectGen(vert, frag, layout);
	}
};


inline OPint OPeffectBind(OPeffect* effect) {
	if (effect == NULL) return OPeffectBind(effect, 0);
	return OPeffectBind(effect, effect->Stride);
}
ui32 OPeffectGetParam(const OPchar* parameterName);

inline OPuint OPeffectParam(const OPchar* param) {
	return glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
}
inline void OPeffectParamf(const OPchar* param, OPfloat f){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform1f(loc, f);
}
inline void OPeffectParamf(OPuint loc, OPfloat f) {
	glUniform1f(loc, f);
}
inline void OPeffectParamfv(const OPchar* param, OPint count, OPfloat* f){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform1fv(loc, count, f);
}
inline void OPeffectParamVec2(const OPchar* param, OPvec2* f){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform2fv(loc, 1, (OPfloat*)f);
}
inline void OPeffectParamVec3(const OPchar* param, OPvec3* f){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform3fv(loc, 1, (OPfloat*)f);
}
inline void OPeffectParamVec4(const OPchar* param, OPvec4* f){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform4fv(loc, 1, (OPfloat*)f);
}
inline void OPeffectParamVec2v(const OPchar* param, OPint count, OPvec2* f){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform2fv(loc, count, (OPfloat*)f);
}
inline void OPeffectParamVec3v(const OPchar* param, OPint count, OPvec3* f){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform3fv(loc, count, (OPfloat*)f);
}
inline void OPeffectParamVec4v(const OPchar* param, OPint count, OPvec4* f){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform4fv(loc, count, (OPfloat*)f);
}
inline void OPeffectParami(const OPchar* param, OPint i){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform1i(loc, i);
}
inline void OPeffectParamBindTex(const OPchar* param, OPtexture* tex){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform1i(loc, OPtextureBind(tex));
}
inline void OPeffectParamBindTex(OPuint loc, OPtexture* tex){
	glUniform1i(loc, OPtextureBind(tex));
}
inline void OPeffectParamBindCubeMap(const OPchar* param, OPtextureCube * tex){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform1i(loc, OPtextureCubeBind(tex));
}
inline void OPeffectParamBindCubeMap(OPuint loc, OPtextureCube * tex){
	glUniform1i(loc, OPtextureCubeBind(tex));
}
inline void OPeffectParamiv(const OPchar* param, OPint count, i32* i){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform1iv(loc, count, i);
}
inline void OPeffectParamMat4(const OPchar* param, OPmat4* matrices){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniformMatrix4fv(loc, 1, GL_FALSE, (OPfloat*)matrices);
}
inline void OPeffectParamMat4(OPuint loc, OPmat4* matrices){
	glUniformMatrix4fv(loc, 1, GL_FALSE, (OPfloat*)matrices);
}
inline void OPeffectParamMat4v(const OPchar* param, OPuint count, OPmat4* matrices){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniformMatrix4fv(loc, count, GL_FALSE, (OPfloat*)matrices);
}


inline void OPeffectParam(OPuint loc, f32 f) {
	OPeffectParamf(loc, f);
}
inline void OPeffectParam(const OPchar* param, f32 f) {
	OPeffectParamf(param, f);
}
inline void OPeffectParam(OPuint loc, OPmat4 matrix){
	OPeffectParamMat4(loc, &matrix);
}
inline void OPeffectParam(const OPchar* param, OPmat4 matrix){
	OPeffectParamMat4(param, &matrix);
}
inline void OPeffectParam(const OPchar* param, OPuint count, OPmat4* matrices){
	OPeffectParamMat4v(param, count, matrices);
}
inline void OPeffectParam(const OPchar* param, OPvec3 f){
	OPeffectParamVec3(param, &f);
}
inline void OPeffectParam(const OPchar* param, OPuint count, OPvec3* f){
	OPeffectParamVec3v(param, count, f);
}
inline void OPeffectParam(const OPchar* param, OPvec4 f){
	OPeffectParamVec4(param, &f);
}
inline void OPeffectParam(const OPchar* param, OPuint count, OPvec4* f){
	OPeffectParamVec4v(param, count, f);
}
inline void OPeffectParam(const OPchar* param, OPtexture* tex){
	OPeffectParamBindTex(param, tex);
}
inline void OPeffectParam(const OPchar* param, OPtextureCube * tex){
	OPeffectParamBindCubeMap(param, tex);
}

inline void OPeffectParam(OPcam* camera) {
	OPeffectParamMat4("uView", &camera->view);
	OPeffectParamMat4("uProj", &camera->proj);
}

#endif
