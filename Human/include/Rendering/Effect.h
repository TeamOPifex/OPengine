#ifndef OPENGINE_HUMAN_RENDERER_EFFECT
#define OPENGINE_HUMAN_RENDERER_EFFECT

#include "./Human/include/Attributes.h"
#include "./Human/include/Utilities/Errors.h"
#include "./Human/include/Rendering/Buffer.h"
#include "./Math/include/Vectors.h"
#include "./Math/include/Matrix4.h"
#include "./Data/include/HashMap.h"
#include "./Data/include/List.h"
#include "./Data/include/File.h"
#include "./Core/include/Types.h"
#include "./Core/include/Log.h"
#include "./Core/include/DynamicMemory.h"

#define OP_EFFECT_NAME_LEN 16


//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _               
// |  __ \                   |  __ \                |  __ \(_)             | | (_)              
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___ 
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/   
typedef GLenum OPshader;

#ifdef OPIFEX_OPENGL_ES_2
#define OPvertexShader   GL_VERTEX_SHADER
#define OPfragmentShader GL_FRAGMENT_SHADER
#else
#define OPvertexShader   GL_VERTEX_SHADER
#define OPfragmentShader GL_FRAGMENT_SHADER
#endif

//-----------------------------------------------------------------------------
//   _____ _                   _       
//  / ____| |                 | |      
// | (___ | |_ _ __ _   _  ___| |_ ___ 
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
typedef struct{
	const OPchar* Name;
	ui32         Type;
	ui32         Elements;
	void*        Offset;
	OPuint		Handle;
}OPshaderAttribute;

typedef struct{
	OPshader  Vertex;
	OPshader  Fragment;
	ui32      ProgramHandle;
	ui32      Stride;
	HashMap*  Parameters;
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
extern OPeffect* OPRENDER_CURR_EFFECT;

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
// Shader creation
OPint OPrenderLoadVertexShader  (const OPchar* filename, OPshader** shader);
OPint OPrenderLoadFragmentShader(const OPchar* filename, OPshader** shader);
// shader destruction
OPint OPrenderUnloadShader(OPshader* shader);

// effect creation
OPeffect OPrenderCreateEffectStride(
	OPshader vert,
	OPshader frag,
	OPshaderAttribute* Attributes,
	OPint AttribCount,
	const OPchar* Name,
	ui32 stride);
OPeffect OPrenderCreateEffect(
	OPshader vert, OPshader frag,
	OPshaderAttribute* Attributes, OPint AttribCount,
	const OPchar* Name);
OPeffect OPrenderGenEffect(
	const OPchar* vert,
	const OPchar* frag,
	ui32 attrs,
	const OPchar* Name,
	ui32 stride);

OPint OPrenderLoadEffect  (const OPchar* filename, OPeffect** effect);

// effect destruction
OPint OPrenderUnloadEffect(OPeffect* effect);

// effect managment
OPint OPrenderBindEffect(OPeffect* effect);
ui32 OPrenderGetParam(const OPchar* parameterName);

// parameter setting
/*void OPrenderParamf(ui32 param, OPfloat f);
void OPrenderParamfv(ui32 param, OPint count, OPfloat* f);
void OPrenderParami(ui32 param, OPint i);
void OPrenderParamiv(ui32 param, OPint count, OPint* i);
void OPrenderParamMat4v(ui32 param, OPint count, OPmat4* matrices);
void OPrenderUseTexture(ui32 param, ui32 uniform, ui32 slot);
*/

inline void OPrenderParamf(const OPchar* param, OPfloat f){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
	);
	glUniform1f(loc, f);
}
inline void OPrenderParamfv(const OPchar* param, OPint count, OPfloat* f){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
	);
	glUniform1fv(loc, count, f);
}
inline void OPrenderParamVec2(const OPchar* param, OPvec2* f){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
		);
	glUniform2fv(loc, 1, (OPfloat*)f);
}
inline void OPrenderParamVec3(const OPchar* param, OPvec3* f){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
		);
	glUniform3fv(loc, 1, (OPfloat*)f);
}
inline void OPrenderParamVec4(const OPchar* param, OPvec4* f){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
		);
	glUniform4fv(loc, 1, (OPfloat*)f);
}
inline void OPrenderParamVec2v(const OPchar* param, OPint count, OPvec2* f){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
		);
	glUniform2fv(loc, count, (OPfloat*)f);
}
inline void OPrenderParamVec3v(const OPchar* param, OPint count, OPvec3* f){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
		);
	glUniform3fv(loc, count, (OPfloat*)f);
}
inline void OPrenderParamVec4v(const OPchar* param, OPint count, OPvec4* f){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
		);
	glUniform4fv(loc, count, (OPfloat*)f);
}
inline void OPrenderParami(const OPchar* param, OPint i){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
	);
	glUniform1i(loc, i);
}
inline void OPrenderParamiv(const OPchar* param, OPint count, i32* i){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
	);
	glUniform1iv(loc, count, i);
}
inline void OPrenderParamMat4(const OPchar* param, OPmat4* matrices){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
		);
	glUniformMatrix4fv(loc, 1, GL_FALSE, (OPfloat*)matrices);
}
inline void OPrenderParamMat4v(const OPchar* param, OPint count, OPmat4* matrices){
	GLuint loc = glGetUniformLocation(
		OPRENDER_CURR_EFFECT->ProgramHandle,
		param
		);
	glUniformMatrix4fv(loc, count, GL_FALSE, (OPfloat*)matrices);
}


#endif
