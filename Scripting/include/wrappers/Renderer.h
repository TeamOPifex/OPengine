#ifndef OPIFEX_SCRIPTING_WRAPPERS_RENDERER
#define OPIFEX_SCRIPTING_WRAPPERS_RENDERER

#ifdef OPIFEX_V8
	
#include "v8.h"
using namespace v8;

void OPscriptRendererWrapper(Isolate* isolate, Handle<ObjectTemplate> global);

#endif

#endif