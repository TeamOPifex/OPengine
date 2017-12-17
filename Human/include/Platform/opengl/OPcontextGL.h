#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL

struct OPcontextGL;
typedef struct OPcontextGL OPcontextGL;

#include "./Human/include/Rendering/API/OPcontext.h"

void OPcontextGLInit(OPcontext* context);

struct OPcontextGL {

};

#endif