#ifndef OPIFEX_HUMAN_UTILITIES_LOADER_OPS
#define OPIFEX_HUMAN_UTILITIES_LOADER_OPS

#include "./Core/include/OPtypes.h"

typedef struct {
	OPchar* data;
#ifdef _DEBUG
	i8 changed;
#endif
} OPscript;

void OPscriptAddLoader();
OPint OPscriptLoad(const OPchar* filename, OPscript** script);
OPint OPscriptUnload(OPscript* script);
OPint OPscriptReload(const OPchar* filename, OPscript** script);
#endif