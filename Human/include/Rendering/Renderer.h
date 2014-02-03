#pragma once
#ifndef OPEngine_Human_Renderer
#define OPEngine_Human_Renderer

#ifdef OPIFEX_OPENGL_ES_2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
#endif

#include "./Core/include/Types.h"

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

extern ui32 OPscreenWidth;
extern ui32 OPscreenHeight;
extern GLFWwindow* window;

#ifdef __cplusplus
};
#endif

#endif
