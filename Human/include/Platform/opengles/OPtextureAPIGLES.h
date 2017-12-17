#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL_ES

struct OPtextureGLES;
typedef struct OPtextureGLES OPtextureGLES;

#include "./Human/include/Rendering/API/OPtextureAPI.h"

void OPtextureAPIGLESInit(OPtextureAPI* texture);

struct OPtextureGLES {
	ui32 Handle;
};

#endif