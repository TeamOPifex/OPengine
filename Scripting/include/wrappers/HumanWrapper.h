#ifndef OPIFEX_SCRIPTING_WRAPPERS_RENDERER
#define OPIFEX_SCRIPTING_WRAPPERS_RENDERER

#ifdef OPIFEX_V8

#include "./Scripting/include/Scripting.h"

void HumanInitializeMethods(V8isolate* isolate, V8ObjectT target);
void HumanInitializeMethodsO(V8isolate* isolate, V8Object target);

#endif

#endif