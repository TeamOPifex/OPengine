#ifndef OPIFEX_SCRIPTING_WRAPPERS_RENDERER
#define OPIFEX_SCRIPTING_WRAPPERS_RENDERER

#ifdef OPIFEX_V8
	
#include "v8.h"
#include "./External/V8/include/libplatform/libplatform.h"
using namespace v8;

void HumanInitializeMethods(Isolate* isolate, Handle<ObjectTemplate> target);

#endif

#endif