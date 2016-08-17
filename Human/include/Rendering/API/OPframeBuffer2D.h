#pragma once

struct OPframeBuffer2D;
typedef struct OPframeBuffer2D OPframeBuffer2D;

#include "./Core/include/OPtypes.h"

struct OPframeBuffer2D {
	void(*Bind)(void* ptr) = 0;
	void(*Unbind)(void* ptr) = 0;
};