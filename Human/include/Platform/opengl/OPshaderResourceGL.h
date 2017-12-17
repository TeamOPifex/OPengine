#pragma once

#include "./Human/include/Platform/OPrendererDefines.h"

#ifdef OPIFEX_OPENGL

struct OPshaderResourceGL;
typedef struct OPshaderResourceGL OPshaderResourceGL;

#include "./Human/include/Rendering/API/OPshaderResource.h"

void OPshaderResourceGLInit(OPshaderResource* shaderResource);

struct OPshaderResourceGL {

};

#endif