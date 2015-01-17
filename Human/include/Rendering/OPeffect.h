#ifndef OPENGINE_HUMAN_RENDERER_EFFECT
#define OPENGINE_HUMAN_RENDERER_EFFECT

#include "./Human/include/Utilities/Errors.h"
#include "./Human/include/Rendering/OPshader.h"
#include "./Human/include/Rendering/OPattributes.h"
#include "./Human/include/Rendering/OPrenderBuffer.h"
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
typedef struct{
	OPshader  Vertex;
	OPshader  Fragment;
	ui32      ProgramHandle;
	ui32      Stride;
	OPhashMap*  Parameters;
	OPlist*   Attributes;
	OPchar    Name[OP_EFFECT_NAME_LEN]; 
}OPeffect;


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

OPint OPeffectUnload(OPeffect* effect);
OPint OPeffectBind(OPeffect* effect);
ui32 OPeffectGetParam(const OPchar* parameterName);

inline void OPrenderParamf(const OPchar* param, OPfloat f){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform1f(loc, f);
}
inline void OPrenderParamfv(const OPchar* param, OPint count, OPfloat* f){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform1fv(loc, count, f);
}
inline void OPrenderParamVec2(const OPchar* param, OPvec2* f){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform2fv(loc, 1, (OPfloat*)f);
}
inline void OPrenderParamVec3(const OPchar* param, OPvec3* f){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform3fv(loc, 1, (OPfloat*)f);
}
inline void OPrenderParamVec4(const OPchar* param, OPvec4* f){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform4fv(loc, 1, (OPfloat*)f);
}
inline void OPrenderParamVec2v(const OPchar* param, OPint count, OPvec2* f){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform2fv(loc, count, (OPfloat*)f);
}
inline void OPrenderParamVec3v(const OPchar* param, OPint count, OPvec3* f){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform3fv(loc, count, (OPfloat*)f);
}
inline void OPrenderParamVec4v(const OPchar* param, OPint count, OPvec4* f){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform4fv(loc, count, (OPfloat*)f);
}
inline void OPrenderParami(const OPchar* param, OPint i){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform1i(loc, i);
}
inline void OPrenderParamiv(const OPchar* param, OPint count, i32* i){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniform1iv(loc, count, i);
}
inline void OPrenderParamMat4(const OPchar* param, OPmat4* matrices){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniformMatrix4fv(loc, 1, GL_FALSE, (OPfloat*)matrices);
}
inline void OPrenderParamMat4v(const OPchar* param, OPint count, OPmat4* matrices){
	GLuint loc = glGetUniformLocation(OPEFFECT_ACTIVE->ProgramHandle, param);
	glUniformMatrix4fv(loc, count, GL_FALSE, (OPfloat*)matrices);
}


#endif
