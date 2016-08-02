#ifndef OPENGINE_HUMAN_RENDERER
#define OPENGINE_HUMAN_RENDERER

#include "./Human/include/Rendering/API/OPrenderer.h"

extern OPrenderer* OPRENDERER_ACTIVE;

#include "./Human/include/Rendering/Enums/OPrendererType.h"
#include "./Math/include/OPvec3.h"
#include "./Math/include/OPvec4.h"
#include "./Core/include/OPtypes.h"

struct OPwindow;


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


//OPwindow* OPrenderCreateWindow(OPmonitor* monitor, bool fullscreen, bool borderless, const OPchar* title, ui32 width, ui32 height);
OPint OPrenderInit();
OPint OPrenderInit(OPrendererType renderer);



inline void  OPrenderClear(f32 r, f32 g, f32 b, f32 a) {
	OPRENDERER_ACTIVE->Clear(r, g, b, a);
}

inline void OPrenderCull(bool state) {
	OPRENDERER_ACTIVE->SetCull(state);
}

inline void OPrenderCullMode(i8 state) {
	OPRENDERER_ACTIVE->SetCullMode(state);
}

inline void  OPrenderSwapBuffer() {
	OPRENDERER_ACTIVE->SwapBuffer();
}

inline void OPrenderPresent() {
	OPRENDERER_ACTIVE->SwapBuffer();
}

inline void OPrenderBlend(bool state) {
	OPRENDERER_ACTIVE->SetBlend(state);
}

inline void OPrenderDepth(bool state) {
	OPRENDERER_ACTIVE->SetDepthTesting(state);
}

inline void OPrenderDepthWrite(bool state) {
	OPRENDERER_ACTIVE->SetDepthWrite(state);
}

inline void OPrenderSetViewport(OPuint x, OPuint y, OPuint width, OPuint height) {
	OPRENDERER_ACTIVE->SetViewport(x, y, width, height);
}


void OPrenderResetViewport(OPuint x, OPuint y, OPuint width, OPuint height);

inline void OPrenderShutdown() {
	OPRENDERER_ACTIVE->Shutdown();
}


inline void  OPrenderClear(f32 r, f32 g, f32 b) {
	OPrenderClear(r, g, b, 1.0f);
}

inline void  OPrenderClear(OPvec3 color) {
	OPrenderClear(color.x, color.y, color.z, 1.0f);
}

inline void  OPrenderClear(OPvec4 color) {
	OPrenderClear(color.x, color.y, color.z, color.w);
}




//void  OPrenderSetScreenSize(ui32 width, ui32 height);
void  OPrenderSetViewport(OPint x, OPint y, ui32 width, ui32 height);
void OPrenderResetViewport();
OPint OPrenderGetWidth();
OPint OPrenderGetHeight();
OPfloat OPrenderGetAspectRatio();
OPfloat OPrenderGetWidthAspectRatio();
OPfloat OPrenderGetHeightAspectRatio();

#endif
