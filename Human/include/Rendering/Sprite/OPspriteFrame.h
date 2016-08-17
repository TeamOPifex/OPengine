#pragma once

struct OPspriteFrame;
typedef struct OPspriteFrame OPspriteFrame;

#include "./Math/include/OPvec2.h"

// Actual sprite offset data
struct OPspriteFrame {
	OPvec2  Offset;
	OPvec2  Size;
	OPfloat Delay; // TODO - implement in exporter
};