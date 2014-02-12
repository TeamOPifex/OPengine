#pragma once
#ifndef OPEngine_Human_Renderer
#define OPEngine_Human_Renderer

#include "./Core/include/Types.h"

#ifdef OPIFEX_OPENGL_ES_2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
#endif

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

extern i32 OPrenderWidth;
extern i32 OPrenderHeight;
extern GLFWwindow* window;

OPint OPrenderInit       (ui32 width, ui32 height, bool fullscreen);
void  OPrenderClear      (f32 r, f32 g, f32 b);
void  OPrenderSetViewport(ui32 x, ui32 y, ui32 width, ui32 height);
OPint OPrenderGetWidth();
OPint OPrenderGetHeight();
OPfloat OPrenderGetAspectRatio(); 
OPfloat OPrenderGetWidthAspectRatio();
OPfloat OPrenderGetHeightAspectRatio();
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

#ifdef __cplusplus
};
#endif

#endif
