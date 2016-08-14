#pragma once

struct OPhashMapBucket;
typedef struct OPhashMapBucket OPhashMapBucket;

#include "./Data/include/OPhashMapPair.h"

struct OPhashMapBucket {
	OPuint count;
	OPhashMapPair *pairs;

	inline OPuint Count() { return count; }
};