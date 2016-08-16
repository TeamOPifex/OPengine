#pragma once

struct OPhashMapPair;
typedef struct OPhashMapPair OPhashMapPair;

#include "./Core/include/OPtypes.h"

struct OPhashMapPair {
	OPchar* key = NULL;
	void* value = NULL;
};