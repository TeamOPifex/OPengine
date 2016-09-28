#pragma once

#include "./Human/include/Rendering/API/OPrenderer.h"

extern OPrenderAPI* OPRENDERER_ACTIVE;

#include "./Human/include/Rendering/Enums/OPrendererType.h"
#include "./Human/include/Rendering/Enums/OPcullFace.h"
#include "./Math/include/OPvec3.h"
#include "./Math/include/OPvec4.h"
#include "./Core/include/OPtypes.h"

#ifdef OPIFEX_DIRECTX_11
    
    #include <windows.h>
    #include <windowsx.h>
    #include <d3d11.h>
    #include <d3d11_1.h>
    
    // include the Direct3D Library file
    #pragma comment (lib, "d3d11.lib")
    extern ID3D11Device* g_pd3dDevice;

#endif

struct OPwindow;

OPint OPrenderSetup();
OPint OPrenderSetup(OPrendererType renderer);
OPint OPrenderInit(OPwindow* window);
void OPrenderResetViewport();
OPint OPrenderGetWidth();
OPint OPrenderGetHeight();
OPfloat OPrenderGetAspectRatio();
OPfloat OPrenderGetWidthAspectRatio();
OPfloat OPrenderGetHeightAspectRatio();

inline void  OPrenderClear(f32 r, f32 g, f32 b, f32 a) {
	OPRENDERER_ACTIVE->Clear(r, g, b, a);
}

inline void  OPrenderClearColor(f32 r, f32 g, f32 b, f32 a) {
	OPRENDERER_ACTIVE->ClearColor(r, g, b, a);
}

inline void  OPrenderClearDepth() {
	OPRENDERER_ACTIVE->ClearDepth();
}


inline void OPrenderCull(bool state) {
	OPRENDERER_ACTIVE->SetCull(state);
}

inline void OPrenderSetWireframe(bool state) {
	OPRENDERER_ACTIVE->SetWireframe(state);
}

inline void OPrenderCullMode(OPcullFace state) {
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

inline void OPrenderSetViewport(ui32 x, ui32 y, ui32 width, ui32 height) {
	OPRENDERER_ACTIVE->SetViewport(x, y, width, height);
}

inline void OPrenderShutdown() {
	OPRENDERER_ACTIVE->Shutdown();
}

inline void  OPrenderClear(f32 r) {
	OPrenderClear(r, r, r, 1.0f);
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