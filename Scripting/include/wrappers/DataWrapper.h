#ifndef OPIFEX_SCRIPTING_WRAPPERS_DATA
#define OPIFEX_SCRIPTING_WRAPPERS_DATA

#ifdef OPIFEX_V8
	
#include "v8.h"
#include "./External/V8/include/libplatform/libplatform.h"
using namespace v8;

void DataInitializeMethods(Isolate* isolate, Handle<ObjectTemplate> target);

#endif

#endif