#ifndef OPENGINE_HUMAN_RENDERING_FRAMEBUFFER2D
#define OPENGINE_HUMAN_RENDERING_FRAMEBUFFER2D

#include "./Core/include/OPtypes.h"

struct OPframeBuffer2D;
typedef struct OPframeBuffer2D OPframeBuffer2D;

struct OPframeBuffer2D {
	void(*Bind)(void* ptr) = 0;
	void(*Unbind)(void* ptr) = 0;
};

#endif