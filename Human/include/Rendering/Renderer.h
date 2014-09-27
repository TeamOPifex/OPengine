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
extern i32 OPscreenWidth;
extern i32 OPscreenHeight;
extern OPint OPrenderFullscreen;
extern OPint OPengineHasFocus;

#ifndef OPIFEX_ANDROID
extern GLFWwindow* window;
#endif

OPint OPrenderInit();
void  OPrenderClear      (f32 r, f32 g, f32 b);
void  OPrenderClearAlpha(f32 r, f32 g, f32 b, f32 a);
void  OPrenderSetScreenSize(OPuint width, OPuint height);
void  OPrenderSetViewport(OPuint x, OPuint y, OPuint width, OPuint height);
void OPrenderResetViewport();
OPint OPrenderGetWidth();
OPint OPrenderGetHeight();
OPfloat OPrenderGetAspectRatio(); 
OPfloat OPrenderGetWidthAspectRatio();
OPfloat OPrenderGetHeightAspectRatio();
void OPrenderCull(OPint state);
void OPrenderCullMode(OPint state);
void  OPrenderSwapBuffer ();
void  OPrenderPresent    ();
void OPrenderBlend(OPint state);
void  OPrenderDepth(OPint state); 
void OPrenderDepthWrite(OPint state);
void  OPrenderShutdown   ();

ui32 OPgetNativeScreenWidth();
ui32 OPgetNativeScreenHeight();

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
