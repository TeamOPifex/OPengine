#pragma once
#ifndef OPEngine_Human_Renderer
#define OPEngine_Human_Renderer

#include "./Core/include/Types.h"

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

OPint OPrenderInit       (ui32 width, ui32 height);
void  OPrenderClear      (f32 r, f32 g, f32 b);
void  OPrenderSetViewport(ui32 x, ui32 y, ui32 width, ui32 height);
void  OPrenderSwapBuffer ();
void  OPrenderPresent    ();
void  OPrenderDepth      (OPint state);
void  OPrenderShutdown   ();

#ifdef __cplusplus
};
#endif

#endif
