#ifndef OPIFEX_SCRIPTING_WRAPPERS_GLOBAL
#define OPIFEX_SCRIPTING_WRAPPERS_GLOBAL

#ifdef OPIFEX_V8
	
#include "v8.h"
#include "./External/V8/include/libplatform/libplatform.h"
using namespace v8;

void GlobalInitializeMethods(Isolate* isolate, Handle<ObjectTemplate> target);

#endif

#endif