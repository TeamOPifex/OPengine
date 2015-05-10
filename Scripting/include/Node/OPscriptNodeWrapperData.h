#ifndef OPENGINE_SCRIPTING_NODE_WRAPPERDATA
#define OPENGINE_SCRIPTING_NODE_WRAPPERDATA

#include "./Core/include/OPtypes.h"
#ifdef OPIFEX_OPTION_NODEJS

#include <node.h>
#include <v8.h>

using namespace v8;

void OPscriptNodeWrapperData(Handle<Object> target);

#endif

#endif //OPIFEXENGINE_OPSCRIPTNODEWRAPPERMATH_H
