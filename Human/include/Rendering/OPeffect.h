#ifndef OPEngine_Human_Renderer_Effect
#define OPEngine_Human_Renderer_Effect
	
#include "./Core/include/DynamicMemory.h"
#include "./Data/include/OPhashMap.h"
#include "./Data/include/OPfile.h"

#include "./Human/include/Rendering/Buffer.h"

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _               
// |  __ \                   |  __ \                |  __ \(_)             | | (_)              
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___ 
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/   
#define ui32 OPshader

#ifdef OPIFEX_OPENGL_ES_2
#define GL_VERTEX_SHADER   OPvertexShader
#define GL_FRAGMENT_SHADER OPfragmentShader
#else
#define GL_VERTEX_SHADER_ARB   OPvertexShader
#define GL_FRAGMENT_SHADER_ARB OPfragmentShader
#endif

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
	OPhashMap Parameters;
	OPlist    Attributes;
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
OPeffect OPrenderCreateEffect(OPshader vert, OPshader frag, OPchar** Attributes, OPint AttribCount);
OPint OPrenderLoadEffect  (const OPchar* filename, OPeffect** effect);

// effect destruction
OPint OPrenderUnloadEffect(OPeffect* effect);

// effect managment
OPint OPrenderBindEffect(OPeffect* effect);
ui32 OPrenderGetParam(const OPchar* parameterName);

// parameter setting
void OPrenderParamf(ui32 param, OPfloat f);
void OPrenderParamfv(ui32 param, OPint count, OPfloat* f);
void OPrenderParami(ui32 param, OPint i);
void OPrenderParamiv(ui32 param, OPint count, OPint* i);
void OPrenderParamMat4v(ui32 param, OPint count, OPmat4* matrices);

// prevent name mangling if compiling with c++
#ifdef __cplusplus
};
#endif

#endif