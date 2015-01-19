#ifndef OPIFEX_SCRIPTING_WRAPPERS_RENDERER
#define OPIFEX_SCRIPTING_WRAPPERS_RENDERER

#include "./OPengineDefines.h"

#ifdef OPIFEX_OPTION_V8

#include "./Scripting/include/OPscript.h"

void HumanInitializeMethodsO(V8isolate* isolate, V8Object target);

#endif

#endif