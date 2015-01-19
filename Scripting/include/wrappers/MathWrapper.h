#ifndef OPIFEX_SCRIPTING_WRAPPERS_MATH
#define OPIFEX_SCRIPTING_WRAPPERS_MATH

#include "./OPengineDefines.h"

#ifdef OPIFEX_OPTION_V8

#include "./Scripting/include/OPscript.h"

void MathInitializeMethods(V8isolate* isolate, V8ObjectTemplate target);
void MathInitializeMethodsO(V8isolate* isolate, V8Object target);

#endif

#endif