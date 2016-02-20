#ifndef OPENGINE_PIPELINE_DEFAULTS
#define OPENGINE_PIPELINE_DEFAULTS

#include "./Data/include/OPcman.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OP_DEFAULT_LOADER_COUNT 7
extern OPassetLoader OP_DEFAULT_LOADERS[OP_DEFAULT_LOADER_COUNT];
void OPloadersAddDefault();

#ifdef __cplusplus
};
#endif

#endif
