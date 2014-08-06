#ifndef OPIFEX_SCRIPTING_WRAPPERS_MATH
#define OPIFEX_SCRIPTING_WRAPPERS_MATH

#ifdef OPIFEX_V8
	
#include "v8.h"
#include "./External/V8/include/libplatform/libplatform.h"
using namespace v8;

void MathInitializeMethods(Isolate* isolate, Handle<ObjectTemplate> target);

#endif

#endif