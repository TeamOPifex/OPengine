#ifndef OPIFEXENGINE_OPSCRIPTNODEWRAPPERCORE
#define OPIFEXENGINE_OPSCRIPTNODEWRAPPERCORE

#include "./Core/include/OPtypes.h"
#ifdef OPIFEX_OPTION_NODEJS

#include <node.h>
#include <v8.h>

using namespace v8;

void OPscriptNodeWrapperCore(Handle<Object> target);

#endif

#endif //OPIFEXENGINE_OPSCRIPTNODEWRAPPERMATH_H
