#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL_ES

struct OPshaderResourceGLES;
typedef struct OPshaderResourceGLES OPshaderResourceGLES;

#include "./Human/include/Rendering/API/OPshaderResource.h"

void OPshaderResourceGLESInit(OPshaderResource* shaderResource);

struct OPshaderResourceGLES {

};

#endif