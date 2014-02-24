#ifndef OPIFEX_HUMAN_UTILITIES_LOADER_OPS
#define OPIFEX_HUMAN_UTILITIES_LOADER_OPS

#include "./Core/include/Types.h"

typedef struct {
	OPchar* data;
} OPscript;

OPint OPscriptLoad(const OPchar* filename, OPscript** script);
OPint OPscriptUnload(OPscript* script);
#endif