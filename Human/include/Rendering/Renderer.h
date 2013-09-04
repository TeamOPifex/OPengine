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
void  OPrenderSetMaterial(MaterialPtr material);
void  OPrenderSetViewport(ui32 x, ui32 y, ui32 width, ui32 height);
void  OPrenderSetBuffer  (OPint bufferType, ui32 buffer);
void  OPrenderTriangles  (ui32 offset, ui32 count);
void  OPrenderTrisIndexed(ui32 numIndices);
void  OPrenderModel      (Model* model);
void  OPrenderPackedModel(PackedModel* model);
void  OPrenderSwapBuffer ();
void  OPrenderPresent    ();
void  OPrenderDepth      (OPint state);
void  OPrenderShutdown   ();

#ifdef __cplusplus
};
#endif

#endif
