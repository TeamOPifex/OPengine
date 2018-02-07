#pragma once

struct OPrenderBufferAPI;
typedef struct OPrenderBufferAPI OPrenderBufferAPI;

#include "./Core/include/OPtypes.h"
#include "./Human/include/Rendering/OPtextureDesc.h"
#include "./Human/include/Rendering/Enums/OPrendererType.h"
struct OPtexture;
struct OPrenderBuffer;

struct OPrenderBufferAPI {
	OPrenderBuffer*(*Create)() = 0;
	OPrenderBuffer*(*Init)(OPrenderBuffer* renderBuffer) = 0;
	void(*Bind)(OPrenderBuffer* ptr) = 0;
	void(*SetSize)(OPrenderBuffer* ptr, ui32 width, ui32 height) = 0;
	void(*Unbind)() = 0;
	void(*Destroy)(OPrenderBuffer* ptr) = 0;
};