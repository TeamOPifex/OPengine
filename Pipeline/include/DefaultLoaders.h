#ifndef OPENGINE_PIPELINE_DEFAULTS
#define OPENGINE_PIPELINE_DEFAULTS

#include "./Data/include/OPcman.h"

#ifdef __cplusplus
extern "C" {
#endif

	extern EXPORT_METHOD OPassetLoader OP_DEFAULT_LOADERS[8];
	EXPORT_METHOD void OPloadersAddDefault();

#ifdef __cplusplus
};
#endif

#endif
