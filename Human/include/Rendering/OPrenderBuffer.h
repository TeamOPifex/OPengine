#pragma once

#include "./Core/include/OPtypes.h"

#include "./Human/include/Rendering/OPvertexBuffer.h"
#include "./Human/include/Rendering/OPindexBuffer.h"

void OPrenderBindBuffer(OPvertexBuffer* buffer);
void OPrenderBindBuffer(OPindexBuffer* buffer);

void OPrenderDrawBufferIndexed(ui32 offset);
void OPrenderDrawBuffer(ui32 offset);
void OPrenderDrawIndexed(ui32 offset, ui32 count);
void OPrenderDrawUserArray(void* vertices, ui32 attrs, ui32 offset, ui32 count);