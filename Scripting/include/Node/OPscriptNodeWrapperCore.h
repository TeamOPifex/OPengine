#ifndef OPENGINE_SCRIPTING_NODE_WRAPPERCORE
#define OPENGINE_SCRIPTING_NODE_WRAPPERCORE

#include "./Core/include/OPtypes.h"
#ifdef OPIFEX_OPTION_NODEJS

#include <node.h>
#include <v8.h>

using namespace v8;

void OPscriptNodeWrapperCore(Handle<Object> target);

#endif

#endif //OPIFEXENGINE_OPSCRIPTNODEWRAPPERMATH_H
