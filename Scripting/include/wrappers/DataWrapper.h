#ifndef OPIFEX_SCRIPTING_WRAPPERS_DATA
#define OPIFEX_SCRIPTING_WRAPPERS_DATA

#ifdef OPIFEX_V8

#include "./Scripting/include/OPscript.h"

void DataInitializeMethods(V8isolate* isolate, V8ObjectT target);
void DataInitializeMethodsO(V8isolate* isolate, V8Object target);

#endif

#endif