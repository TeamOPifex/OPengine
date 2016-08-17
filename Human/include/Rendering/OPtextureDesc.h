#pragma once

struct OPtextureDesc;
typedef struct OPtextureDesc OPtextureDesc;

#include "./Human/include/Rendering/Enums/OPtextureFilter.h"
#include "./Human/include/Rendering/Enums/OPtextureFormat.h"
#include "./Human/include/Rendering/Enums/OPtextureWrap.h"
#include "./Core/include/OPtypes.h"

struct OPtextureDesc {
	ui32 width;
	ui32 height;
	OPtextureFormat format;
	OPtextureWrap wrap;
	OPtextureFilter filter;
};