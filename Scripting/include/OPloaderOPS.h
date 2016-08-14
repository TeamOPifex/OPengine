#pragma once

#include "./Core/include/OPtypes.h"
#include "./Data/include/OPstream.h"

struct OPscript {
	OPchar* data;
	OPchar* filename;
#ifdef _DEBUG
	i8 changed;
#endif
};

void OPscriptAddLoader();
OPint OPscriptLoad(OPstream* str, OPscript** script);
OPint OPscriptUnload(OPscript* script);
OPint OPscriptReload(OPstream* str, OPscript** script);
