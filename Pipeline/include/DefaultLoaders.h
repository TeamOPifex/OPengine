#ifndef OPENGINE_PIPELINE_DEFAULTS
#define OPENGINE_PIPELINE_DEFAULTS

#include "./Data/include/ContentManager.h"

#ifdef __cplusplus
extern "C" {
#endif

extern OPassetLoader OP_DEFAULT_LOADERS[10];
void OPloadersAddDefault();

#ifdef __cplusplus
};
#endif

#endif
