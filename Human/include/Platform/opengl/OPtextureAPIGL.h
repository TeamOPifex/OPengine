#pragma once

struct OPtextureGL;
typedef struct OPtextureGL OPtextureGL;

#include "./Human/include/Rendering/API/OPtextureAPI.h"

void OPtextureAPIGLInit(OPtextureAPI* texture);

struct OPtextureGL {
	ui32 Handle;
};