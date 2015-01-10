#ifndef OPENGINE_HUMAN_RENDERER
#define OPENGINE_HUMAN_RENDERER

#include "./Human/include/Rendering/OpenGL.h"
#include "./Core/include/OPtypes.h"

extern IMPORT_METHOD i32 OPRENDER_WIDTH;
extern IMPORT_METHOD i32 OPRENDER_HEIGHT;
extern IMPORT_METHOD i32 OPRENDER_SCREEN_WIDTH;
extern IMPORT_METHOD i32 OPRENDER_SCREEN_HEIGHT;
extern IMPORT_METHOD f32 OPRENDER_SCREEN_WIDTH_SCALE;
extern IMPORT_METHOD f32 OPRENDER_SCREEN_HEIGHT_SCALE;
extern IMPORT_METHOD OPint OPRENDER_FULLSCREEN;
extern IMPORT_METHOD OPint OPRENDER_HAS_FOCUS;

#ifndef OPIFEX_ANDROID
extern IMPORT_METHOD GLFWwindow* window;
#endif

EXPORT_METHOD OPint OPrenderInit();
EXPORT_METHOD void  OPrenderClear(f32 r, f32 g, f32 b);
EXPORT_METHOD void  OPrenderClearAlpha(f32 r, f32 g, f32 b, f32 a);
EXPORT_METHOD void  OPrenderSetScreenSize(OPuint width, OPuint height);
EXPORT_METHOD void  OPrenderSetViewport(OPuint x, OPuint y, OPuint width, OPuint height);
EXPORT_METHOD void OPrenderResetViewport();
EXPORT_METHOD OPint OPrenderGetWidth();
EXPORT_METHOD OPint OPrenderGetHeight();
EXPORT_METHOD OPfloat OPrenderGetAspectRatio();
EXPORT_METHOD OPfloat OPrenderGetWidthAspectRatio();
EXPORT_METHOD OPfloat OPrenderGetHeightAspectRatio();

/* Enables or Disables GL_CULL_FACE
 * @param state 0 for disable or 1 for enable
*/
EXPORT_METHOD void OPrenderCull(OPint state);

/* Sets the direction of culling
 * @param state 0 for back or 1 for front
*/
EXPORT_METHOD void OPrenderCullMode(OPint state);
EXPORT_METHOD void  OPrenderSwapBuffer();
EXPORT_METHOD void  OPrenderPresent();
EXPORT_METHOD void OPrenderBlend(OPint state);
EXPORT_METHOD void  OPrenderDepth(OPint state);
EXPORT_METHOD void OPrenderDepthWrite(OPint state);
EXPORT_METHOD void  OPrenderShutdown();

EXPORT_METHOD ui32 OPgetNativeScreenWidth();
EXPORT_METHOD ui32 OPgetNativeScreenHeight();

inline void OPrenderBlendAlpha(){
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
}
inline void OPrenderBlendAdditive(){
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); 
}

#endif
