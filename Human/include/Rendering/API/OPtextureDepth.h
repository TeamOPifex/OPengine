#pragma once

struct OPtextureDepth;
typedef struct OPtextureDepth OPtextureDepth;

struct OPtextureDepth {
	void(*Bind)(void* ptr) = 0;
	void(*Unbind)(void* ptr) = 0;
};