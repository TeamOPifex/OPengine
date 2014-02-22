#ifndef OPENGINE_HUMAN_RENDERER
#define OPENGINE_HUMAN_RENDERER

#include "./Human/include/Rendering/OpenGL.h"
#include "./Core/include/Types.h"

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

extern i32 OPrenderWidth;
extern i32 OPrenderHeight;

#ifndef OPIFEX_ANDROID
extern GLFWwindow* window;
#endif

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
