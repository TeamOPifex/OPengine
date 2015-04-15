#ifndef OPENGINE_PIPELINE_DEFAULTS
#define OPENGINE_PIPELINE_DEFAULTS

#include "./Data/include/OPcman.h"

#ifdef __cplusplus
extern "C" {
#endif

extern OPassetLoader OP_DEFAULT_LOADERS[6];
void OPloadersAddDefault();

#ifdef __cplusplus
};
#endif

#endif
