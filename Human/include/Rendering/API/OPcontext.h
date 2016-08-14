#pragma once

struct OPcontext;
typedef struct OPcontext OPcontext;

#include "./Core/include/OPtypes.h"

struct OPcontext {
	void(*Bind)(void* ptr) = 0;
	void(*Unbind)(void* ptr) = 0;
};