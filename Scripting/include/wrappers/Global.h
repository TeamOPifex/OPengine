#ifndef OPIFEX_SCRIPTING_WRAPPERS_GLOBAL
#define OPIFEX_SCRIPTING_WRAPPERS_GLOBAL

#include "./OPengineDefines.h"

#ifdef OPIFEX_OPTION_V8

#include "./Scripting/include/OPscript.h"

void GlobalInitializeMethods(V8isolate* isolate, V8ObjectTemplate target);
void GlobalInitializeMethodsO(V8isolate* isolate, V8Object target);

#endif

#endif