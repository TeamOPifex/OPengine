#ifndef OPIFEX_SCRIPTING_V8
#define OPIFEX_SCRIPTING_V8

#include "./Scripting/include/wrappers/V8Types.h"

#ifdef OPIFEX_V8

#include "v8.h"
#include "./include/libplatform/libplatform.h"
using namespace v8;

extern V8isolate* isolate;
#endif

void OPscriptInit();

#endif