#pragma once

struct OPframeBufferDepth;
typedef struct OPframeBufferDepth OPframeBufferDepth;

#include "./Core/include/OPtypes.h"

struct OPframeBufferDepth {
	void(*Bind)(void* ptr) = 0;
	void(*Unbind)(void* ptr) = 0;
};