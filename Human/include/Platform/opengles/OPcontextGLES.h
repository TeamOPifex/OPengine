#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL_ES

struct OPcontextGLES;
typedef struct OPcontextGLES OPcontextGLES;

#include "./Human/include/Rendering/API/OPcontext.h"

void OPcontextGLESInit(OPcontext* context);

struct OPcontextGLES {

};

#endif