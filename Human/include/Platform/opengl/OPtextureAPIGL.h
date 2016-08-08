#ifndef OPENGINE_HUMAN_RENDERING_TEXTURE_GL
#define OPENGINE_HUMAN_RENDERING_TEXTURE_GL

#include "./Human/include/Rendering/API/OPtextureAPI.h"

void OPtextureAPIGLInit(OPtextureAPI* texture);

struct OPtextureGL {
	ui32 Handle;
};

#endif