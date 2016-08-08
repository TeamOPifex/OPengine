#ifndef OPENGINE_HUMAN_RENDERING_MONITOR_API
#define OPENGINE_HUMAN_RENDERING_MONITOR_API

#include "./Core/include/OPtypes.h"

struct OPmonitorAPI;
typedef struct OPmonitorAPI OPmonitorAPI;

struct OPmonitor;
struct OPmonitorResult;

struct OPmonitorAPI {
	OPmonitorResult(*GetAll)() = 0;
};

#endif