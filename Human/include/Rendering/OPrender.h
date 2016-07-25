#ifndef OPENGINE_HUMAN_RENDERER
#define OPENGINE_HUMAN_RENDERER

#include "./Human/include/Rendering/API/OPrenderer.h"

enum OPRENDERER {
	OPRENDERER_OPENGL,
	OPRENDERER_DIRECTX
};

extern OPrenderer* OPRENDERER_ACTIVE;

#include "./Human/include/Rendering/OpenGL.h"
#include "./Math/include/OPvec3.h"
#include "./Math/include/OPvec4.h"
#include "./Core/include/OPtypes.h"

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
extern void (*OP_WINDOW_DROP)(int, const OPchar**);
void OPrenderDragAndDropCB(void (*cb)(int, const OPchar**));
#endif

#ifdef OPIFEX_DIRECTX_11

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3d11_1.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
extern ID3D11Device* g_pd3dDevice;

#endif

#include "./Human/include/Rendering/OPwindow.h"

//OPwindow* OPrenderCreateWindow(OPmonitor* monitor, bool fullscreen, bool borderless, const OPchar* title, ui32 width, ui32 height);
OPint OPrenderInit(OPwindow* window);
OPint OPrenderInit(OPwindow* window, OPRENDERER renderer);


void  OPrenderClear(f32 r, f32 g, f32 b, f32 a);
void OPrenderCull(bool state);
void OPrenderCullMode(i8 state);
void  OPrenderSwapBuffer();
void  OPrenderPresent();
void OPrenderBlend(bool state);
void  OPrenderDepth(bool state);
void OPrenderDepthWrite(bool state);
void  OPrenderShutdown();


inline void  OPrenderClear(f32 r, f32 g, f32 b) {
	OPrenderClear(r, g, b, 1.0f);
}
inline void  OPrenderClear(OPvec3 color) {
	OPrenderClear(color.x, color.y, color.z, 1.0f);
}
inline void  OPrenderClear(OPvec4 color) {
	OPrenderClear(color.x, color.y, color.z, color.w);
}

inline void OPrenderBlendAlpha() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
inline void OPrenderBlendAdditive() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}





void  OPrenderSetScreenSize(ui32 width, ui32 height);
void  OPrenderSetViewport(OPint x, OPint y, ui32 width, ui32 height);
void OPrenderResetViewport();
OPint OPrenderGetWidth();
OPint OPrenderGetHeight();
OPfloat OPrenderGetAspectRatio();
OPfloat OPrenderGetWidthAspectRatio();
OPfloat OPrenderGetHeightAspectRatio();

#endif
