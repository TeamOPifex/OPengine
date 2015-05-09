#ifndef OPIFEXENGINE_OPSCRIPTNODEWRAPPERMATH_H
#define OPIFEXENGINE_OPSCRIPTNODEWRAPPERMATH_H

#include "./Core/include/OPtypes.h"
#ifdef OPIFEX_OPTION_NODEJS

#include <node.h>
#include <v8.h>

using namespace v8;

void OPscriptNodeWrapperMath(Handle<Object> target);

#endif

#endif //OPIFEXENGINE_OPSCRIPTNODEWRAPPERMATH_H
