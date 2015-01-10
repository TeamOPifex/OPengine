#ifndef OPIFEX_SCRIPTING_WRAPPERS_PERFORMANCE
#define OPIFEX_SCRIPTING_WRAPPERS_PERFORMANCE

#ifdef OPIFEX_V8

#include "./Scripting/include/OPscript.h"

void PerformanceInitializeMethods(V8isolate* isolate, V8ObjectT target);
void PerformanceInitializeMethodsO(V8isolate* isolate, V8Object target);

#endif

#endif