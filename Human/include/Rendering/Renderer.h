#pragma once
#ifndef OPEngine_Human_Renderer
#define OPEngine_Human_Renderer

#include "./Core/include/Types.h"

#ifdef OPIFEX_OPENGL_ES_2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#include <GL/glfw.h>
//#include <glm/glm.hpp>
#endif

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

extern OPint OPrenderWidth;
extern OPint OPrenderHeight;

OPint OPrenderInit       (ui32 width, ui32 height);
void  OPrenderClear      (f32 r, f32 g, f32 b);
void  OPrenderSetViewport(ui32 x, ui32 y, ui32 width, ui32 height);
void  OPrenderSwapBuffer ();
void  OPrenderPresent    ();
void  OPrenderDepth      (OPint state);
void  OPrenderShutdown   ();

inline void OPrenderBlendAlpha(){
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
}
inline void OPrenderBlendAdditive(){
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); 
}


extern ui32 OPscreenWidth;
extern ui32 OPscreenHeight;
#ifdef __cplusplus
};
#endif

#endif
