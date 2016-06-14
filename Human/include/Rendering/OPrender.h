#ifndef OPENGINE_HUMAN_RENDERER
#define OPENGINE_HUMAN_RENDERER

#include "./Human/include/Rendering/OpenGL.h"
#include "./Math/include/OPvec3.h"
#include "./Math/include/OPvec4.h"
#include "./Core/include/OPtypes.h"

extern i8 OPRENDER_INITIALIZED;
extern ui32 OPRENDER_WIDTH;
extern ui32 OPRENDER_HEIGHT;
extern ui32 OPRENDER_SCREEN_WIDTH;
extern ui32 OPRENDER_SCREEN_HEIGHT;
extern ui32 OPRENDER_SCALED_WIDTH;
extern ui32 OPRENDER_SCALED_HEIGHT;
extern f32 OPRENDER_SCREEN_WIDTH_SCALE;
extern f32 OPRENDER_SCREEN_HEIGHT_SCALE;
extern i8 OPRENDER_FULLSCREEN;
extern i8 OPRENDER_HAS_FOCUS;
extern OPuint OPRENDER_VAO;

#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS)
extern GLFWwindow* window;
extern void (*OP_WINDOW_DROP)(int, const OPchar**);
void OPrenderDragAndDropCB(void (*cb)(int, const OPchar**));
#endif

#include "./Human/include/Rendering/OPwindow.h"

OPwindow* OPrenderCreateWindow(OPmonitor* monitor, bool fullscreen, const OPchar* title, ui32 width, ui32 height);
OPint OPrenderInit();
void  OPrenderClear(f32 r, f32 g, f32 b, f32 a);
void  OPrenderSetScreenSize(ui32 width, ui32 height);
void  OPrenderSetViewport(OPint x, OPint y, ui32 width, ui32 height);
void OPrenderResetViewport();
OPint OPrenderGetWidth();
OPint OPrenderGetHeight();
OPfloat OPrenderGetAspectRatio();
OPfloat OPrenderGetWidthAspectRatio();
OPfloat OPrenderGetHeightAspectRatio();

/* Enables or Disables GL_CULL_FACE
 * @param state 0 for disable or 1 for enable
*/
void OPrenderCull(OPint state);

/* Sets the direction of culling
 * @param state 0 for back or 1 for front
*/
void OPrenderCullMode(OPint state);
void  OPrenderSwapBuffer ();
void  OPrenderPresent    ();
void OPrenderUpdate();
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

//inline OPint OPrenderInit() {
//	return OPrenderInit(OPRENDER_WIDTH, OPRENDER_HEIGHT);
//}
inline void  OPrenderClear(f32 r, f32 g, f32 b) {
	OPrenderClear(r,g,b,1.0f);
}
inline void  OPrenderClear(OPvec3 color) {
	OPrenderClear(color.x, color.y, color.z, 1.0f);
}
inline void  OPrenderClear(OPvec4 color) {
	OPrenderClear(color.x, color.y, color.z, color.w);
}

#endif
