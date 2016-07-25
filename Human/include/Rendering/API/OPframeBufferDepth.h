#ifndef OPENGINE_HUMAN_RENDERING_FRAMEBUFFER_DEPTH
#define OPENGINE_HUMAN_RENDERING_FRAMEBUFFER_DEPTH

#include "./Core/include/OPtypes.h"

struct OPframeBufferDepth;
typedef struct OPframeBufferDepth OPframeBufferDepth;

struct OPframeBufferDepth {
	void(*Bind)(void* ptr) = 0;
	void(*Unbind)(void* ptr) = 0;
};

#endif